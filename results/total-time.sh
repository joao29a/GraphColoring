#!/bin/bash

file=$1
total_ms=0
total_sec=0
total_min=0
total_colors=0
while read i; do
    colors=$(echo $i | cut -d ' ' -f5)
    let total_colors=$total_colors+$colors
    time=$(echo $i | cut -d '(' -f2 | cut -d ')' -f1)
    ms=$(echo $time | cut -d ' ' -f3 | sed -e s/[a-z]//g)
    sec=$(echo $time | cut -d ' ' -f2 | sed -e s/[a-z]//g)
    min=$(echo $time | cut -d ' ' -f1 | sed -e s/[a-z]//g)
    let total_ms=$total_ms+$ms
    let total_sec=$total_sec+$sec
    let total_min=$total_min+$min
done < $file
let ms_sec=$total_ms/1000
let min_sec=$total_min*60
let total_sec=$total_sec+$min_sec+$ms_sec
echo "$total_sec seconds"
echo "$total_colors colors"
