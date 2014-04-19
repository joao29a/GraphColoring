#!/bin/bash

prog=color.out
tabu=15
candidates=10000
iterations=10000

run-all(){
    time for i in $(find ./input/ -type f | sort -V); do
        time-one $i $tabu $candidates $iterations
    done
}


time-all(){
    time for i in $(find ./input/ -type f); do 
        time-one $i $1 $2
    done
}

time-one(){
    { time ./$prog $1 $tabu $candidates $iterations > results/$1 ; }\
        2>> results/$1
    echo; 
}

perf-all(){
    for i in $(find ./input/ -type f); do
        perf $1 $i 
    done
}

#FIXME
perf(){
    psrun -c $1.xml -o $2 -F text ./$prog $2 $3 $4
    mv ./input/*.txt ./
    for file in $(find . -name *.txt -type f); do
        let total_lines=$(cat $1.md | wc -l)+1
        cat $file | grep -A$total_lines Index | grep -v -e = -e Index > temp
        rm $file
        i=$(echo $2 | sed -e s/.input// -e s/.//)
        i=$(echo $i | sed -e s/nput//)
        mkdir -p hardware/result$i
        touch hardware/result$i/$file
        while read -r fileDes <&1 && read -r fileTxt <&2; do
            fileTxt=$(echo $fileTxt | cut -d " " -f3)
            echo -e "$fileDes $(printf "%'d" $fileTxt)" >> hardware/result$i/$file
        done 1<$1.md 2<temp
        rm temp
    done
}

clean(){
    for i in $(find ./input/*.txt -type f); do
        new=$(echo $i | sed -e s/.txt//)
        mv $i $new
    done
}

$1 $2 $3 $4 $5
