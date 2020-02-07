#!/bin/bash
pathExe=$1
values=(5)

for i in "${values[@]}";
do
    echo "Test:"$i

    ./gerador 50 35 20 30 30 > T$i.in

    ts=$(date +%s%N)
    ./$pathExe < T$i.in > /dev/null
    echo $((($(date +%s%N) - $ts)/1000000)) milisseconds

    #rm T$i.in
done