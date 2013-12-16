#!/usr/bin/python
import os
import thread, threading
import sys
import time
import subprocess

# program usage: python queueManager.py <input file> <output file>

signal_file = "signals.txt"
detection_program = "./detect_sb"
dataExtract_program = "./extract_data"
exitFlag = 0 # be sure to set global exitFlag
time_interval = 90

class QueueNode():
	def __init__(self, data = None, Next = None, timestamp = time.time()):
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
			self.front = QueueNode(data)
			self.back = self.front
		else:
			self.back.Next = QueueNode(data)
			self.back = self.back.Next
		self.size += 1
	def dequeue(self):
		if (self.front == None):
			return None
		else:
			data = self.front.data
			self.front = self.front.Next
			self.size -= 1
			return data
	def peek(self):
		if (self.front == None):
			return None
		else:
			return self.front.data

class DataObject():
	def __init__(self):
		self.data = "None"
		self.lock = threading.Lock()

class ImageBuilder(threading.Thread):
	def __init__(self, threadID, queue):
		threading.Thread.__init__(self)
		self.index = 0
		self.queue = queue
	def run(self):
		global exitFlag
		while not exitFlag:
			imageFilename = "img" + str(self.index) + ".jpg"

			# hold and wait for a new image
			if not imageFilename in os.listdir("."):
				continue

			# once image has been found, add to queue, as well as remove extra stuff
			self.queue.lock.acquire()
			self.queue.enqueue(imageFilename)
			if (self.queue.size > 100):
				fname = self.queue.dequeue()
				if fname in os.listdir("."):
					os.system("rm " + fname) # clears the folder
			self.queue.lock.release()
			self.index += 1

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
			if time.time() - imageFilename.timestamp > time_interval:
				continue
			if imageFilename != None:
				try:
					global detection_program
					output = subprocess.check_output([detection_program, imageFilename]).decode("utf-8")
					if not "scoreboard" in output:
						self.extractQueue.lock.acquire()
						self.extractQueue.enqueue("./scoreboards/" + imageFilename[1:]) # this is an error in the filename, please resolve later in detect_sb.cpp
						self.extractQueue.lock.release()
				except Exception, e:
					print "\nError has occurred within the second thread's subprocess", imageFilename
					print str(e)
					print ""
				os.system("rm " + imageFilename) # clears the folder

class DataExtract(threading.Thread):
	def __init__(self, threadID, extractQueue, dataObject):
		threading.Thread.__init__(self)
		self.threadID = threadID
		self.extractQueue = extractQueue
		self.dataObject = dataObject
	def run(self):
		global exitFlag
		while not exitFlag:
			self.extractQueue.lock.acquire()
			imageFilename = self.extractQueue.dequeue()
			self.extractQueue.lock.release()
			if time.time() - imageFilename.timestamp > time_interval:
				continue
			if imageFilename != None:
				try:
					global dataExtract_program
					output = subprocess.check_output([dataExtract_program, imageFilename, "gameData.txt"]).decode("utf-8")
					# be sure to output the game data
					while not "gameData.txt" in os.listdir("."):
						continue
					FILE = open("gameData.txt", "r")
					self.dataObject.lock.acquire()
					self.dataObject.data = FILE.read()
					self.dataObject.lock.release()
					FILE.close()
					os.system("rm gameData.txt")
				except Exception, e:
					print "\nError has occurred within the third thread's subprocess", imageFilename
					print str(e)
					print ""
				#os.system("rm " + imageFilename)

def main():
	args = [arg for arg in sys.argv]

	inputFilename = args[1]
	print "Currently not in use but will be in the future (input file): inputFilename"
	outputFilename = args[2]
	outputFile = None
	if outputFilename != "stdout":
		outputFile = open(outputFilename, "w")

	image_queue = Queue()
	extract_queue = Queue()
	dataObject = DataObject()
	
	if signal_file not in os.listdir("."):
		os.system("touch " + signal_file)

	buildingThread = ImageBuilder(1, image_queue)
	detectionThread = ScoreboardDetect(2, image_queue, extract_queue)
	extractionThread = DataExtract(3, extract_queue, dataObject)

	# start the threads
	buildingThread.start()
	detectionThread.start()
	extractionThread.start()

	while True:
		signalsFile = open(signal_file, "r")
		if signalsFile != None:
			signals = signalsFile.read()
			signalsFile.close()
			signalsFile = open(signal_file, "w")
			signalsFile.write("")
			signalsFile.close()
			if "kill" in signals:				# be sure to have "kill" in the signals file
				# close the threads
				print "Closing threads..."
				global exitFlag
				exitFlag = True
				buildingThread.join()
				detectionThread.join()
				extractionThread.join()
				print "Done closing threads. Exiting."
				break
		dataObject.lock.acquire()
		if dataObject.data != "None":
			if outputFile == None:
				print dataObject.data
			else:
				outputFile.write(dataObject.data)
			dataObject.data = "None"
		dataObject.lock.release()
	outputFile.close()

if __name__ == "__main__":
	main()
