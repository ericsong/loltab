#!/usr/bin/env python
from __future__ import print_function
from livestreamer import Livestreamer, StreamError, PluginError, NoPluginError

import gobject
gobject.threads_init()
import pygst
pygst.require("0.10")
import gst
import sys
import os
import thread, threading
import sys
import time
import subprocess

# program usage: python queueManager.py <input file> <output file>

signal_file = "signals.txt"
alert_program = "./alert"
detection_program = "./detect_sb"
dataExtract_program = "./extract_data"
exitFlag = 0 # be sure to set global exitFlag
time_interval = 90
inGame = True

def exit(msg):
    sys.exit()

def onPad(obj, pad, target):
    sinkpad = target.get_compatible_pad(pad, pad.get_caps())
    if sinkpad:
        pad.link(sinkpad)
        return True

class LivestreamerPlayer(object):
    def __init__(self):
        self.fd = None
        self.mainloop = gobject.MainLoop()

        self.pipeline = gst.element_factory_make("playbin2", None)
        self.pipeline.set_property("uri", "appsrc://")

        self.parsebin = gst.Pipeline()
        videorate = gst.element_factory_make("videorate", None)
        videorate.set_property("max-rate", 4) 
        jpegenc = gst.element_factory_make("jpegenc", None)
        multifilesink = gst.element_factory_make("multifilesink", None)
        multifilesink.set_property("location", "img%d.jpg")
        self.parsebin.add(videorate)
        self.parsebin.add(jpegenc)       
        self.parsebin.add(multifilesink) 
        videorate.link(jpegenc)      
        jpegenc.link(multifilesink)
        
        pad = videorate.get_pad("sink");
        ghost_pad = gst.GhostPad("sink", pad);
        self.parsebin.add_pad(ghost_pad);

        self.pipeline.set_property("video-sink", self.parsebin)
 
#        self.pipeline.cconnect("need-data", self.on_source_need_data)
        self.pipeline.connect("source-setup", self.on_source_setup)

        # Creates a bus and set callbacks to receive errors
        self.bus = self.pipeline.get_bus()
        self.bus.add_signal_watch()
        self.bus.connect("message::eos", self.on_eos)
        self.bus.connect("message::error", self.on_error)

    def exit(self, msg):
        self.stop()
        exit(msg)

    def stop(self):
        # Stop playback and exit mainloop
        self.pipeline.set_state(gst.STATE_NULL)
        self.mainloop.quit()

        # Close the stream
        if self.fd:
            self.fd.close()

    def play(self, stream):
        # Attempt to open the stream
        try:
            self.fd = stream.open()
        except StreamError as err:
            self.exit("Failed to open stream: {0}".format(err))

        # Start playback
        print("set to playing")
        self.pipeline.set_state(gst.STATE_PLAYING)

    def on_source_setup(self, element, source):
        # When this callback is called the appsrc expects
        # us to feed it more data
        source.connect("need-data", self.on_source_need_data)

    def on_source_need_data(self, source, length):
        # Attempt to read data from the stream
        try:
            data = self.fd.read(length)
        except IOError as err:
            self.exit("Failed to read data from stream: {0}".format(err))

        # If data is empty it's the end of stream
        if not data:
            source.emit("end-of-stream")
            return

        # Convert the Python bytes into a GStreamer Buffer
        # and then push it to the appsrc
        buf = gst.Buffer(data)
        source.emit("push-buffer", buf)

    def on_eos(self, bus, msg):
        # Stop playback on end of stream
        self.stop()

    def on_error(self, bus, msg):
        # Print error message and exit on error
        error = msg.parse_error()[1]
        self.exit(error)

def getStream(url, quality):
    livestreamer = Livestreamer()
    livestreamer.set_loglevel("info")
    livestreamer.set_logoutput(sys.stdout)

    # Attempt to find a plugin for this URL
    try:
        plugin = livestreamer.resolve_url(url)
    except NoPluginError:
        exit("Livestreamer is unable to handle the URL '{0}'".format(url))

    # Attempt to fetch streams
    try:
        streams = plugin.get_streams()
    except PluginError as err:
        exit("Plugin error: {0}".format(err))

    if len(streams) == 0:
        exit("No streams found on URL '{0}'".format(url))

    # Look for specified stream
    if quality not in streams:
        exit("Unable to find '{0}' stream on URL '{1}'".format(quality, url))

    # We found the stream
    stream = streams[quality]

    return stream

class QueueNode():
	def __init__(self, data = None, Next = None, timestamp = 0):
		self.data = data
		self.Next = Next
		self.timestamp = timestamp

class Queue():
	def __init__(self):
		self.front = None
		self.back = None
		self.size = 0
		self.lock = threading.Lock()
	def enqueue(self, data):
		if (self.front == None):
			self.front = QueueNode(data, None, time.time())
			self.back = self.front
		else:
			self.back.Next = QueueNode(data, None, time.time())
			self.back = self.back.Next
		self.size += 1
	def dequeue(self):
		if (self.front == None):
			return None
		else:
			data = (self.front.data, self.front.timestamp)
			self.front = self.front.Next
			self.size -= 1
			return data
	def peek(self):
		if (self.front == None):
			return None
		else:
			return (self.front.data, self.front.timestamp)

class ImageBuilder(threading.Thread):
	def __init__(self, threadID, queue):
		threading.Thread.__init__(self)
		self.index = 0
		self.queue = queue
		self.threadID = threadID
	def run(self):
		global exitFlag
		while not exitFlag:
			imageFilename = "img" + str(self.index) + ".jpg"

			# hold and wait for a new image
			if not imageFilename in os.listdir("."):
				continue
			else:
				self.index += 1

			try:
				global inGame
				global alert_program
				#print ("Found " + imageFilename)
				if not inGame:
					output = subprocess.check_output([alert_program, imageFilename]).decode("utf-8")
					print("not in game")
					if "true" in output:
						inGame = True
						# need generation of the names.txt file via something (a call possibly?)
					os.system("rm " + imageFilename)
				else:
					# once image has been found, add to queue, as well as remove extra stuff
					self.queue.lock.acquire()
					self.queue.enqueue(imageFilename)
					"""if (self.queue.size > 100):
						fname = self.queue.dequeue()
						if fname in os.listdir("."):
							os.system("rm " + fname) # clears the folder"""
					self.queue.lock.release()
			except Exception, e:
				print ("\nError has occurred within the first thread's subprocess " + imageFilename)
				print (str(e) + "\n")

class ScoreboardDetect(threading.Thread):
	def __init__(self, threadID, imageQueue, extractQueue):
		threading.Thread.__init__(self)
		self.threadID = threadID
		self.imageQueue = imageQueue
		self.extractQueue = extractQueue
	def run(self):
		global exitFlag
		while not exitFlag:
			self.imageQueue.lock.acquire()
			imageFilename = self.imageQueue.dequeue()
			self.imageQueue.lock.release()
			if imageFilename != None:
				#print("Received image")
				if (time.time() - imageFilename[1]) > time_interval:
					print ("ERROR: time issue", time.time() - imageFilename[1])
					continue
				try:
					global detection_program
					output = subprocess.check_output([detection_program, imageFilename[0]]).decode("utf-8")
					if not "scoreboard" in output:
						print("SCOREBOARD FOUND")
						self.extractQueue.lock.acquire()
						self.extractQueue.enqueue("./scoreboards/" + imageFilename[0])
						self.extractQueue.lock.release()
				except Exception, e:
					print ("\nError has occurred within the second thread's subprocess", imageFilename[0])
					print (str(e) + "\n")
				os.system("rm " + imageFilename[0]) # clears the folder"""

class DataExtract(threading.Thread):
	def __init__(self, threadID, extractQueue, outputFilename):
		threading.Thread.__init__(self)
		self.threadID = threadID
		self.extractQueue = extractQueue
		self.outputFilename = outputFilename
	def run(self):
		global exitFlag
		while not exitFlag:
			self.extractQueue.lock.acquire()
			imageFilename = self.extractQueue.dequeue()
			self.extractQueue.lock.release()
			if imageFilename != None:
				if time.time() - imageFilename[1] > time_interval:
					continue
				try:
					global dataExtract_program
					print("GRABBED SCOREBOARD TO ANALYZE")
					output = subprocess.check_output([dataExtract_program, imageFilename[0], self.outputFilename]).decode("utf-8")
					# be sure to output the game data
					while not self.outputFilename in os.listdir("."):
						continue
					print("FINISHED ANALYSIS")
				except Exception, e:
					print ("\nError has occurred within the third thread's subprocess", imageFilename[0])
					print (str(e) + "\n")
				os.system("rm " + imageFilename[0])

def main():
	args = [arg for arg in sys.argv]

	if len(args) < 4:
		print ("Invalid usage: ./program <streamer name> <medium|high|best> <output file> [T(force ingame)]\n example: ./queueManager flosd best stdout T")

	url = "twitch.tv/" + args[1]
	quality = args[2]

	outputFilename = args[3]

	if len(args) == 5:
		global inGame
		inGame = True

	image_queue = Queue()
	extract_queue = Queue()
	
	if "scoreboards" not in os.listdir("."):
		os.system("mkdir scoreboards")

	buildingThread = ImageBuilder(1, image_queue)
	detectionThread = ScoreboardDetect(2, image_queue, extract_queue)
	extractionThread = DataExtract(3, extract_queue, outputFilename)

	# start the threads
	buildingThread.start()
	detectionThread.start()
	extractionThread.start()

	# We found the stream
	stream = getStream(url, quality)

	# Create the player and start playback
	player = LivestreamerPlayer()

	# Blocks until playback is done
	player.play(stream)

	while True:
		signalsFile = open(signal_file, "r")
		if signalsFile != None:
			signals = signalsFile.read()
			signalsFile.close()
			signalsFile = open(signal_file, "w")
			signalsFile.write("")
			signalsFile.close()
			if "kill" in signals:
				# close the threads
				print ("Closing threads...")
				global exitFlag
				exitFlag = True
				buildingThread.join()
				detectionThread.join()
				extractionThread.join()
				player.stop()
				print ("Done closing threads. Exiting.")
				break

if __name__ == "__main__":
	main()
