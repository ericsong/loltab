import requests, sys

message = { "name" : sys.argv[1] }
url = "0.0.0.0:8000/raise_scoreboard_event" # fill this in yo
requests.post(url, data = json.dumps(message), headers = {"content-type": "application/json"})
