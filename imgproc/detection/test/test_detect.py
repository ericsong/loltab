import os
import sys
import subprocess

imgdir = sys.argv[1];

for img in os.listdir(imgdir):
    imgpath = imgdir + img

    output = subprocess.check_output(['./detect_sb', imgpath]).decode('utf-8')

    if('scoreboard' not in output):
        print(img + ":   " + output)
