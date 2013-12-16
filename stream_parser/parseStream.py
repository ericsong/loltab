#!/usr/bin/python

import sys
import pygst
pygst.require("0.10")
import gst
from livestreamer import Livestreamer

livestreamer = Livestreamer()

streamer = "http://twitch.tv/" + sys.argv[1]

plugin = livestreamer.resolve_url(streamer)

streams = plugin.get_streams()
stream = streams.get('best')



class Main:
    def __init__(self):
        self.pipeline = gst.Pipeline("mypipeline")

        self.audiotestsrc = gst.element_factory_make("audiotestsrc", "audio")
        self.pipeline.add(self.audiotestsrc)

        self.sink = gst.element_factory_make("autoaudiosink", "sink")
        self.pipeline.add(self.sink)

        self.audiotestsrc.link(self.sink)
        
        self.pipeline.set_state(gst.STATE_PLAYING)

start=Main()
