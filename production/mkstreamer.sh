#!/bin/bash

# USAGE: ./mkstreamer.sh <streamer name> <output file | stdout>

# create folder for the streamer

cd streams
if [ -d $1 ]
then
	rm -rf $1
fi
mkdir $1
cp -rf ../model/* $1/
cd $1/
if test "$2" != "stdout"
then
	touch $2
fi
