import os
import subprocess

for img in os.listdir('img'):
    imgpath = 'img/' + img

    output = subprocess.check_output(['./detect_sb', imgpath]).decode('utf-8')

    if('top' in output):
        print(imgpath)
        print(output)
