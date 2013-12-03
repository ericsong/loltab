import os
import subprocess

for img in os.listdir('img'):
    imgpath = 'img/' + img

    output = subprocess.check_output(['./detect_sb', imgpath]).decode('utf-8')

    if('scoreboard' in output):
        print(imgpath)
