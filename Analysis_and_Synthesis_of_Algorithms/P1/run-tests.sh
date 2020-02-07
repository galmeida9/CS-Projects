#!/bin/bash
#Usage: ./run-tests PATH_TO_EXEC
pathExe=$1
pathTests="tests/"
numberTests=$(ls $pathTests/ | grep ".in" | cut -f 1 -d ".")
testRight=0
testTotal=0
unpassedTests=""

for i in $numberTests;
do
    test=$i
	echo /--------
	echo "Test:"$test
	ts=$(date +%s%N)
	./$pathExe < $pathTests/$i.in > $pathTests/out_$i.txt
	echo $((($(date +%s%N) - $ts)/1000000)) milisseconds
    DIFF=$(diff $pathTests/out_$i.txt $pathTests/$i.out)
    testTotal=$(echo $testTotal+1 | bc)
    if [ "$DIFF" = "" ]
	then
		testRight=$(echo $testRight+1 | bc)
		echo Test Passed
	else
		echo Test Unpassed
		unpassedTests+=$test"\n"
	fi

    echo \\--------
	echo
    rm $pathTests/out_$i.txt
done