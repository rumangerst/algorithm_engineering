#!/bin/bash

EXECUTABLE=$1
PARAM=$2
TYPE=$3

num=9999999999999

for ((i=1; i<=40;i++)); do

	value=$($EXECUTABLE $PARAM | grep $TYPE | egrep -o '[0-9]+')
	
	if [ "$value" -lt "$num" ]; then
		num=$value
	fi
done

echo $num
