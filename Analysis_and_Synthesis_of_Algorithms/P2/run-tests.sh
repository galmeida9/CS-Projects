#!/bin/bash

#Usage: ./run-tests PATH_TO_EXEC

pathExe=$1
pathTests="tests/"
numberTests=$(ls $pathTests/ | grep "T")
testRight=0
testTotal=0
unpassedTests=""

for i in $numberTests;
do
    test=$i
	echo /--------
	echo "Test:"$test
	ts=$(date +%s%N)
	./$pathExe < $pathTests/$i/input > $pathTests/$i/out_.txt
	echo $((($(date +%s%N) - $ts)/1000000)) milisseconds
    DIFF=$(diff $pathTests/$i/out_.txt $pathTests/$i/output)
	echo $DIFF
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
    rm $pathTests/$i/out_.txt
done

echo $testRight/$testTotal = $(echo $testRight*100 /$testTotal | bc)%
echo -e $unpassedTests