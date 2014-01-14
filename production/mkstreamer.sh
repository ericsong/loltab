#!/bin/bash

# USAGE: ./mkstreamer.sh <streamer name> <output file | stdout>

# create folder for the streamer

cd streams
if [ -d $1 ]
then
	rm -rf $1
fi
mkdir $1
for fname in ../model/*
do
	if [ -f "$fname" ]
	then
		ln -s ../model/$fname $1/
	fi
done
cd $1/
mkdir "scoreboards"
if test "$2" != "stdout"
then
	touch $2
fi
