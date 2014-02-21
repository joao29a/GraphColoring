#!/bin/bash

get_time(){
    time for i in $(find ./input/ -type f); do 
        echo $i; time ./out $i; 
        echo; 
    done
}

perf(){
    for i in $(find ./input/ -type f); do
        psrun -c hardware/system.xml -o $i -F text ./out $i -p
        mv ./input/*.txt ./
        for file in $(find . -name *.txt -type f); do
            cat $file | grep -A21 Index | grep -v -e = -e Index > temp
            rm $file
            i=$(echo $i | sed -e s/.input// -e s/.//)
            mkdir -p hardware/result$i
            touch hardware/result$i/$file
            while read -r fileDes <&1 && read -r fileTxt <&2; do
                fileTxt=$(echo $fileTxt | cut -d " " -f3)
                echo -e "$fileDes $(printf "%'d" $fileTxt)" >> hardware/result$i/$file
            done 1<hardware/description.md 2<temp
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

$1
