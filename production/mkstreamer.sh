#!/bin/bash

# USAGE: ./mkstreamer.sh <streamer name> <output file | stdout>

# create folder for the streamer

metaFile="metadata.txt"

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
		ln -s $fname ../streams/$1/
		echo "ln -s $fname ../streams/$1/"
	else
		if [ -d "$fname" ]
		then
			mkdir ../streams/$1/$fname
			echo "mkdir ../streams/$1/$fname"
		else
			cp $fname ../streams/$1/
			echo "cp $fname ../streams/$1/"
		fi
	fi
done
cd ../streams/$1/
if test "$2" != "stdout"
then
	touch $2
fi
