#!/bin/sh

set -Eeuo pipefail

outFile=output/latency.txt
a=0
loops=10

# Create folders and files
mkdir -p output
touch $outFile
echo -n | tee $outFile

# Custom duration
while getopts l: flag
do
	case "${flag}" in
		l) loops=${OPTARG};;
	esac
done

while [ $a -lt $loops ]
do
	sleep 1

	date '+%s%N' | cut -b1-13 >> $outFile
	curl -s localhost:8080/hello
	date '+%s%N' | cut -b1-13 >> $outFile
	echo "" >> $outFile

	a=`expr $a + 1`

done

make latency
./latencyCalc
