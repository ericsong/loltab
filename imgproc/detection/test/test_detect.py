import os
import subprocess

for img in os.listdir('/home/reggi/tmp/froggen720p/'):
    imgpath = '/home/reggi/tmp/froggen720p/' + img

    output = subprocess.check_output(['./detect_sb', imgpath]).decode('utf-8')

    if('scoreboard' not in output):
        print(img + ":   " + output)
