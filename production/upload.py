import json
from pymongo import MongoClient

client = MongoClient('localhost', 27017);
db = client.loltabber
scoreboards = db.scoreboards

while True:
    f = open('upload_pipe')
    jsonstring = ""
    for line in f.readlines():
        if("!!!END!!!" in line):
            decoded = json.loads(jsonstring.strip())
            decoded['streamer'] = 'TheOddOne'
            print(decoded)
            #print(scoreboards.insert(decoded))
        elif("!!!START!!!" in line):
            jsonstring = ""
        else:
            jsonstring += line
