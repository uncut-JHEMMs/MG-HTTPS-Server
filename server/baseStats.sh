#!/bin/sh

#set -Eeuo pipefail

outFile=output/InitialStats.txt
outFileCache=output/InitialStats_Cache.txt
outFileCall=output/InitialStats_Call.txt
outFileMassif=output/InitialStats_Massif.txt
sep="--------------------------------------------------"

getStats() {
	echo "/proc/meminfo" > $1
	echo $sep >> $1
	cat /proc/meminfo >> $1
	echo $sep >> $1

	echo "" >> $1
	echo "/proc/cpuinfo" >> $1
	echo $sep >> $1
	cat /proc/cpuinfo | grep -m 1 "model name" >> $1
	cat /proc/cpuinfo | grep -m 1 "cpu cores" >> $1
	echo $sep >> $1

	echo "" >> $1
	echo "free" >> $1
	echo $sep >> $1
	free -h >> $1
	echo $sep >> $1

	echo "" >> $1
	echo "top" >> $1
	echo $sep >> $1
	top -p $2 -b -n 1 >> $1
	echo $sep >> $1

	echo "" >> $1
	echo "vmstat" >> $1
	echo $sep >> $1
	vmstat -s >> $1
	echo "" >> $1
	vmstat >> $1
	echo $sep >> $1

	echo "" >> $1
	echo "iostat" >> $1
	echo $sep >> $1
	iostat -m >> $1
	echo "" >> $1
	iostat -x >> $1
	echo $sep >> $1

	echo "" >> $1
	echo "mpstat" >> $1
	echo $sep >> $1
	mpstat -P ALL >> $1
	echo $sep >> $1

	echo "Printed statistics to $1"
}

make

# Base stats
./service &
sleep 1
pid=$!

curl -s localhost:8080/hello
getStats $outFile $pid
kill $pid
sleep 1


# Cachegrind stats
valgrind --tool=cachegrind --cachegrind-out-file=output/cachegrind.out ./service &
sleep 1
pid=$!

curl -s localhost:8080/hello
getStats $outFileCache $pid
kill $pid
sleep 1

echo "" >> $outFileCache
echo "cachegrind" >> $outFileCache
cg_annotate output/cachegrind.out >> $outFileCache
echo $sep >> $outFileCache


# Callgrind stats
valgrind --tool=callgrind --callgrind-out-file=output/callgrind.out ./service &
sleep 1
pid=$!

curl -s localhost:8080/hello
getStats $outFileCall $pid
kill $pid
sleep 1

echo "" >> $outFileCall
echo "callgrind" >> $outFileCall
callgrind_annotate output/callgrind.out >> $outFileCall
echo $sep >> $outFileCall

# Massif stats
valgrind --tool=massif --stacks=yes --xtree-memory=full --xtree-memory-file=output/xtmemory.kcg --massif-out-file=output/massif.out ./service &
sleep 1
pid=$!

curl -s localhost:8080/hello
getStats $outFileMassif $pid
kill $pid
sleep 1

echo "" >> $outFileMassif
echo "massif" >> $outFileMassif
ms_print output/massif.out >> $outFileMassif
echo "" >> $outFileMassif

echo "xtree" >> $outFileMassif
callgrind_annotate --auto=yes --inclusive=yes output/xtmemory.kcg >> $outFileMassif
echo $sep >> $outFileMassif
