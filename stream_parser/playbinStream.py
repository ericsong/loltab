#!/usr/bin/env python2

from __future__ import print_function
from livestreamer import Livestreamer, StreamError, PluginError, NoPluginError

import gobject
gobject.threads_init()

import pygst
pygst.require("0.10")

import gst
import sys

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
        self.pipeline.set_property("volume", 0.0)
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


def main():
    if len(sys.argv) < 3:
        exit("Usage: {0} <url> <quality>".format(sys.argv[0]))

    # Collect arguments
    url = sys.argv[1]
    quality = sys.argv[2] #set quality to 'best'
    
    # We found the stream
    stream = getStream(url, quality)

    # Create the player and start playback
    player = LivestreamerPlayer()

    # Blocks until playback is done
    player.play(stream)
    for i in range(10000000):
        print("hi")

if __name__ == "__main__":
    main()
