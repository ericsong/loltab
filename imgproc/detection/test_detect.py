import os
import subprocess

for img in os.listdir('img'):
    imgpath = 'img/' + img

    subprocess.call(['./detect_sb', imgpath])
