#!/bin/bash
if [ -z $1 ] || [ -z $2 ];
then
	echo "Usage: writer <full path to file> <phrase>"
	exit 1
fi

mkdir -p "$(dirname "$1")"
if [ $? -ne 0 ];
then
    echo "Could create directory entries"
    exit 1
fi

#create an empty file
touch $1
if [ $? -ne 0 ];
then
    echo "Could not create file"
    exit 1
fi

#out phrase to file
echo $2 > $1
exit 0