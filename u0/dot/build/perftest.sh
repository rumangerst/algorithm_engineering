#!/bin/bash

EXECUTABLE=$1
PARAM=$2

num=9999999999999

for ((i=1; i<=1000;i++)); do

	value=$($EXECUTABLE $PARAM | tail -n1 | egrep -o '[0-9]+')
	
	if [ "$value" -lt "$num" ]; then
		num=$value
		
		echo "New best runtime: $value"
	fi
done

echo $num
