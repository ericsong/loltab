from livestreamer import Livestreamer
livestreamer = Livestreamer()

plugin = livestreamer.resolve_url("http://twitch.tv/tsm_dyrus")

streams = plugin.get_streams()
stream = streams.get('best')

print(streams)

for stream in streams:
    print(stream)
