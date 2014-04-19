#!/bin/bash

prog=color.out
checker=checker.out
system=hardware/system.xml
system_md=hardware/system.md
conf=tabu.conf
tabu=0
candidates=0
iterations=0
threads=0
repeat=0
items=$(cat $conf)

for i in $items; do
    key=$(echo $i | cut -d '=' -f1)
    value=$(echo $i | cut -d '=' -f2)
    if [ $key == "tabu" ]; then
        tabu=$value
    elif [ $key == "candidates" ]; then
        candidates=$value
    elif [ $key == "iterations" ]; then
        iterations=$value
    elif [ $key == "threads" ]; then
        threads=$value
    elif [ $key == "repeat" ]; then
        repeat=$value
    fi
done

run-all(){
    for i in $(find ./input/ -type f | sort -V); do
        old=$i
        for loop in `seq 1 $repeat`; do
            i=$old
            perf $i $tabu $candidates $iterations
        done
    done
}

perf(){
    printf "%-40s: " $i
    init=$(date +%s%3N)
    psrun -c $system -o $1 -F text ./$prog $1 $2 $3 $4 > temp
    let end=$(date +%s%3N)-$init
    let ms=$end%1000
    let sec=$end/1000
    let min=$sec/60
    let sec=$sec%60
    let total=$total+$end
    cat temp | ./$checker $i
    printf "(%'dm %'ds %'dms)\n" $min $sec $ms
    rm temp
    mv ./input/*.txt ./
    for file in $(find . -name *.txt -type f); do
        let total_lines=$(cat $system_md | wc -l)+1
        cat $file | grep -A$total_lines Index | grep -v -e = -e Index > temp
        rm $file
        i=$(echo $1 | sed -e s/.input// -e s/.//)
        i=$(echo $i | sed -e s/nput//)
        mkdir -p hardware/result$i
        touch hardware/result$i/$file
        while read -r fileDes <&1 && read -r fileTxt <&2; do
            fileTxt=$(echo $fileTxt | cut -d " " -f3)
            echo -e "$fileDes $(printf "%'d" $fileTxt)" >> hardware/result$i/$file
        done 1<$system_md 2<temp
        printf "time: (%'dm %'ds %'dms)\n" $min $sec $ms\
            >> hardware/result$i/$file
        rm temp
    done
}

run-all
