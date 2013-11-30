import json
from pymongo import MongoClient

client = MongoClient('localhost', 27017);
db = client.loltabber
scoreboards = db.scoreboards

while True:
    f = open('/dev/shm/loltab_pipe')
    jsonstring = ""
    for line in f.readlines():
        if("!!!!!" in line):
            decoded = json.loads(jsonstring)
            print(scoreboards.insert(decoded))
            jsonstring = ""
        else:
            jsonstring += line
