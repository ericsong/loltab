#!/bin/bash

# USAGE: ./compileModel.sh

# compile and move all programs to the model folder !!! NOTE: THIS WILL REMOVE THE MODEL FOLDER !!!

cacheName="CMakeCache.txt"
rm -rf model/
mkdir model
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
cp ../managers/queueManager.py model/

cp ../name_scraper/name_scraper.js model/
cp ../name_scraper/metadata.txt model/
cp ../imgproc/upload/upload.py model/

cd model/
mkdir "scoreboards"
