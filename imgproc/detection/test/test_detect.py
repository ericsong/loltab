import os
import subprocess

for img in os.listdir('../img/720p'):
    imgpath = '../img/720p/' + img

    output = subprocess.check_output(['./detect_sb', imgpath]).decode('utf-8')

    print(output)
