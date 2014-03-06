#!/bin/bash

prog=color.out

time-all(){
    time for i in $(find ./input/ -type f); do 
        time-one $i
    done
}

time-one(){
    echo $1; time ./$prog $1; 
    echo; 
}

perf-all(){
    for i in $(find ./input/ -type f); do
        perf $1 $i
    done
}

perf(){
    psrun -c $1.xml -o $2 -F text ./$prog $2
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

$1 $2 $3
