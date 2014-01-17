#!/bin/bash

# USAGE: ./mkstreamer.sh <streamer name> <output file | stdout>

# create folder for the streamer

metaFile="metadata.txt"

if [ $# -ne 1 ]
then
	echo "usage: ./mkstreamer.sh <streamer name>"
	exit 1
fi

cd streams
if [ -d $1 ]
then
	rm -rf $1
fi
mkdir $1
cd ../model/
for fname in *
do
	if [ -f "$fname" ] && [ "$fname" != "$metaFile" ]
	then
		ln $fname ../streams/$1/
		echo "ln $fname ../streams/$1/"
	elif [ -d "$fname" ]
	then
		mkdir ../streams/$1/$fname
		echo "mkdir ../streams/$1/$fname"
	elif [ -p "$fname" ]
	then
		ln $fname ../streams/$1/
		echo "ln $fname ../streams/$1/"
	else
		cp $fname ../streams/$1/
		echo "cp $fname ../streams/$1/"
	fi
done
cd ../streams/$1/
if [ -p "upload_pipe" ]
then
	echo ""
else
	mkfifo "upload_pipe"
	echo "mkfifo upload_pipe"
fi
