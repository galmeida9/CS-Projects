#!/bin/bash
n_threads=$1
path=$2
results='results/'
speedupString='.speedups.csv'
path_time=$path".res"
path_res="$results${path#*/}$speedupString"

rm -f $path_res
echo '#threads, exec_time, speedup' >> $path_res

./CircuitRouter-SeqSolver/CircuitRouter-SeqSolver $path
string=($(sed '4q;d' $path_time))
seqTime=${string[3]}
echo 1S, $seqTime, 1 >> $path_res

for i in $(seq 1 $n_threads)
do
	./CircuitRouter-ParSolver/CircuitRouter-ParSolver -t $i $path
    string=($(sed '4q;d' $path_time))
    parTime=${string[3]}
	speedup=$(echo "scale=6; ${seqTime}/${parTime}" | bc)
    echo $i, $parTime, ${speedup} >> $path_res
done

cat $path_res
