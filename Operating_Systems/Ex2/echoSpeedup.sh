#!/bin/bash
for f in $(ls */*.csv)
do 
    echo [$f]: $(sed '6q;d' $f)
done
