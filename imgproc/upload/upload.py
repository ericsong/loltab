import json
from pymongo import MongoClient

client = MongoClient('localhost', 27017);
db = client.loltabber
scoreboards = db.scoreboards

metadata = {};

#listen for upload writes
while True:
    f = open('upload_pipe')
    jsonstring = ""
    for line in f.readlines():
        print(line)
        if("!!!UPDATE!!!"):
            mdfile = open('metadata.txt')
            mdstring = ""
            for line in mdfile.readlines():
                mdstring += line
            metadata = json.loads(mdstring)
            print("Metadata updated")
            print(metadata) 
        if("!!!END!!!" in line):
            decoded = json.loads(jsonstring.strip())
            decoded['streamer'] = 'meteos'
            print(decoded)
        elif("!!!START!!!" in line):
            jsonstring = ""
        else:
            jsonstring += line
