#!/bin/bash

# USAGE: ./compileModel.sh

# compile and move all programs to the model folder

cacheName="CMakeCache.txt"
cd ../imgproc/
cd alert/
rm -rf $cacheName
cmake . && make
mv alert ../../production/model/
cd ../dataextract/
rm -rf $cacheName
cmake . && make
mv bin/extract_data ../../production/model/
cd ../detection/main
rm -rf $cachenae
cmake . && make
mv detect_sb ../../../production/model/
cd ../../../production/
cp queueManager.py model/
cd model/
mkdir "scoreboards"
