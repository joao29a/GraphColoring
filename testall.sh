#!/bin/bash

prog=color.out

get_time(){
    time for i in $(find ./input/ -type f); do 
        echo $i; time ./$prog $i; 
        echo; 
    done
}

perf(){
    for i in $(find ./input/ -type f); do
        psrun -c $1.xml -o $i -F text ./$prog $i
        mv ./input/*.txt ./
        for file in $(find . -name *.txt -type f); do
            total_lines=$(cat $1.md | wc -l)
            cat $file | grep -A$total_lines Index | grep -v -e = -e Index > temp
            rm $file
            i=$(echo $i | sed -e s/.input// -e s/.//)
            mkdir -p hardware/result$i
            touch hardware/result$i/$file
            while read -r fileDes <&1 && read -r fileTxt <&2; do
                fileTxt=$(echo $fileTxt | cut -d " " -f3)
                echo -e "$fileDes $(printf "%'d" $fileTxt)" >> hardware/result$i/$file
            done 1<$1.md 2<temp
            rm temp
        done
    done
}

clean(){
    for i in $(find ./input/*.txt -type f); do
        new=$(echo $i | sed -e s/.txt//)
        mv $i $new
    done
}

$1 $2
