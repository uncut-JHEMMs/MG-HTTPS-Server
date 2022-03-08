#!/bin/bash

set -Eeuo pipefail

outputFolder=output
freeMem=$outputFolder/freemem.txt
usedMem=$outputFolder/usedmem.txt
freeSwap=$outputFolder/freeswap.txt
usedSwap=$outputFolder/usedswap.txt
topCpu=$outputFolder/topcpu.txt
topMem=$outputFolder/topmem.txt
pid=$(pgrep service)
a=0
loops=10

# Create folders and files
mkdir -p $outputFolder
touch $usedMem $freeMem $usedSwap $freeSwap $topCpu $topMem
echo -n | tee $usedMem $freeMem $usedSwap $freeSwap $topCpu $topMem

# Print statistics 'loops' times
while [ $a -lt $loops ]
do
	sleep 1

	free | awk 'NR==2{print $3}' >> $usedMem
	free | awk 'NR==2{print $4}' >> $freeMem
	free | awk 'NR==3{print $3}' >> $usedSwap
	free | awk 'NR==3{print $4}' >> $freeSwap
	
	top -p $pid -b -n 1 | awk '$12 == "service"{print $9}' >> $topCpu
	top -p $pid -b -n 1 | awk '$12 == "service"{print $10}' >> $topMem

	a=`expr $a + 1`

done

# Plot used memory
gnuplot -e "set terminal png size 600,400; set output '$outputFolder/usedmem.png';
	set title 'Used memory over time';
	set xlabel 'Time (s)';
	set ylabel 'Used memory (KiB)';
	set key off;
	plot '$usedMem' w lines"