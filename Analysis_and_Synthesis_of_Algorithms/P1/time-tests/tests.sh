#!/bin/bash
pathExe=$1
values=(5 6 7 8 9 10)

for i in "${values[@]}";
do
    test=$(($i*10**6))

	echo "Test:"$i
    nA=$(echo $test/2 | bc)

    subRegions=$(( ( RANDOM % $nA ) + 2 ))
    nM=$(echo $nA/$subRegions+1 | bc)
    ./gerador $nA $nA $subRegions 1 $nM > T$i.in

    ts=$(date +%s%N)
    ./$pathExe < T$i.in > /dev/null
    echo $((($(date +%s%N) - $ts)/1000000)) milisseconds

    rm T$i.in
done