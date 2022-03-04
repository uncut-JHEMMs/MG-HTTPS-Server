#!/bin/sh

set -Eeuo  pipefail

sep="--------------------------------------------------"
pid=$(pgrep service)

echo "/proc/meminfo"
echo $sep
cat /proc/meminfo
echo $sep

echo ""
echo "/proc/cpuinfo"
echo $sep
cat /proc/cpuinfo | grep -m 1 "model name"
cat /proc/cpuinfo | grep -m 1 "cpu cores"
echo $sep

echo ""
echo "free"
echo $sep
free -h
echo $sep

echo ""
echo "top"
echo $sep
top -p $pid -b -n 1
echo $sep

echo ""
echo "vmstat"
echo $sep
vmstat -s
echo ""
vmstat
echo $sep

echo ""
echo "iostat"
echo $sep
iostat -m
echo ""
iostat -x
echo $sep

echo ""
echo "mpstat"
echo $sep
mpstat -P ALL
echo $sep

