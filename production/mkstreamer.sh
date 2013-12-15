#!/bin/bash

# create folder for the streamer
cacheName="CMakeCache.txt"
scoreboards="scoreboards"

cd streams
if [ -d $1 ]
then
	rm -rf $1
fi
mkdir $1
cd ../../imgproc/dataextract/
rm -f $cachename
cmake . && make
cp bin/extract_data ../../production/streams/$1/
cd ../detection/main/
rm -f $cacheName
cp detect_sb ../../../production/streams/$1/
cd ../../../production/
cp queueManager.py streams/$1/
cd streams/$1/
mkdir $scoreboards
if test "$2" != "stdout"
then
	touch $2
fi
