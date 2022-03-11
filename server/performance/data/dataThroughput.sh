#!/bin/sh

set -Eeuo pipefail

latOut=tmp/ioping.out
summary=DataSummary.txt

echo "Printing lsblk stats"
echo "lsblk" > $summary
lsblk >> $summary
echo "" >> $summary

echo "Printing blkid stats"
echo "blkid" >> $summary
blkid >> $summary
echo "" >> $summary

echo "Running ioping test"
echo "Latency test" >> $summary
ioping -c 10 . > $latOut
tail -n 4 $latOut >> $summary
echo "" >> $summary

# Sequential writes with 1Mb block size. Imitates write backup activity or large file copies.
echo "Running sequential write fio test"
echo "Sequential write fio test" >> $summary
fio --name=fiotest --filename=/test/test1 --size=2Gb --rw=write --bs=1M --direct=1 --numjobs=8 --ioengine=libaio --iodepth=8 --group_reporting --runtime=60 --startdelay=60 > tmp/fio1.out
awk '/IOPS/ {print $2,$3,$4}' tmp/fio1.out >> $summary
tail -n 2 tmp/fio1.out >> $summary
echo "" >> $summary

# Sequential reads with 1Mb block size. Imitates read backup activity or large file copies.
echo "Running sequential read fio test"
echo "Sequential read fio test" >> $summary
fio --name=fiotest --filename=/test/test2 --size=2Gb --rw=read --bs=1M --direct=1 --numjobs=8 --ioengine=libaio --iodepth=8 --group_reporting --runtime=60 --startdelay=60 > tmp/fio2.out
awk '/IOPS/ {print $2,$3,$4}' tmp/fio2.out >> $summary
tail -n 2 tmp/fio2.out >> $summary
echo "" >> $summary

# Random writes with 64Kb block size. Medium block size workload for writes.
echo "Running random write fio test"
echo "Random write fio test" >> $summary
fio --name=fiotest --filename=/test/test3 --size=2Gb --rw=randwrite --bs=64k --direct=1 --numjobs=8 --ioengine=libaio --iodepth=16 --group_reporting --runtime=60 --startdelay=60 > tmp/fio3.out
awk '/IOPS/ {print $2,$3,$4}' tmp/fio3.out >> $summary
tail -n 2 tmp/fio3.out >> $summary
echo "" >> $summary

# Random reads with 64Kb block size. Medium block size workload for reads.
echo "Running random read fio test"
echo "Random read fio test" >> $summary
fio --name=fiotest --filename=/test/test4 --size=2Gb --rw=randread --bs=64k --direct=1 --numjobs=8 --ioengine=libaio --iodepth=16 --group_reporting --runtime=60 --startdelay=60 > tmp/fio4.out
awk '/IOPS/ {print $2,$3,$4}' tmp/fio4.out >> $summary
tail -n 2 tmp/fio4.out >> $summary
echo "" >> $summary

# Random writes with 8Kb block size. Common database workload simulation for writes.
echo "Running random write small block fio test"
echo "Random write small block fio test" >> $summary
fio --name=fiotest --filename=/test/test5 --size=2Gb --rw=randwrite --bs=8k --direct=1 --numjobs=8 --ioengine=libaio --iodepth=32 --group_reporting --runtime=60 --startdelay=60 > tmp/fio5.out
awk '/IOPS/ {print $2,$3,$4}' tmp/fio5.out >> $summary
tail -n 2 tmp/fio5.out >> $summary
echo "" >> $summary

# Random reads with 8Kb block size. Common database workload simulation for reads.
echo "Running random read small block fio test"
echo "Random read small block fio test" >> $summary
fio --name=fiotest --filename=/test/test6 --size=2Gb --rw=randread --bs=8k --direct=1 --numjobs=8 --ioengine=libaio --iodepth=32 --group_reporting --runtime=60 --startdelay=60 > tmp/fio6.out
awk '/IOPS/ {print $2,$3,$4}' tmp/fio6.out >> $summary
tail -n 2 tmp/fio6.out >> $summary
