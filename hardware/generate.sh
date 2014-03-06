#!/bin/bash

filexml=$1.xml
filedesc=$1.md

echo "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>" > $filexml
echo "<ps_hwpc_eventlist class=\"PAPI\">" >> $filexml

rm -f $filedesc
touch $filedesc

num=0
papi_avail | grep -i papi_ | while read i; do
    name=$(echo $i | awk '{print $1}')
    avail=$(echo $i | awk '{print $3}')
    desc=$(echo $i | cut -d ' ' -f5-)
    if [ $avail == "Yes" ]; then
        echo "  <ps_hwpc_event type=\"preset\" name=\"$name\" />" >> $filexml
        echo "$num $desc" >> $filedesc
        let num=$num+1
    fi
done

echo "</ps_hwpc_eventlist>" >> $filexml
