import sys

from livestreamer import Livestreamer
livestreamer = Livestreamer()

streamer = "http://twitch.tv/" + sys.argv[1]

plugin = livestreamer.resolve_url(streamer)

streams = plugin.get_streams()
stream = streams.get('best')

print(streams)

for stream in streams:
    print(stream)
