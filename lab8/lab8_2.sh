#!/bin/bash

RESULT=`ls /usr/bin/ | awk 'NR%101==204607621%101'`
IFS=$'\n'
OUTPUT=""

for i in $RESULT
do
	OUTPUT=`ldd /usr/bin/$i`
	printf '%s\n\n'"$OUTPUT"
done > test2.txt