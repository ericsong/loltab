from livestreamer import Livestreamer
livestreamer = Livestreamer()

plugin = livestreamer.resolve_url("http://twitch.tv/sirhcez")

streams = plugin.get_streams()
stream = streams.get('best')


fd = stream.open()
files = []

for vidc in range(3):
    name = "test" + str(vidc) + ".flv"
    fp = open(name, 'wb')
    files.append(fp)  
    for x in range(3000):
        data = fd.read(1024)
        fp.write(data)
    fp.close()
    

fd.close()
