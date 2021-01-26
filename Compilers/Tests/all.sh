#!/bin/bash
path=$1
runPath=$path/og
testRight=0
testTotal=0
unpassedTests=""

for d in $(ls -d */ | grep "T-"); do
	cd $d
	echo "----------"
	echo "Folder: $d"
	for f in $(ls *.og)	
	do
		test=$(echo ${f::-3})
		echo /--------
		echo $test
		echo "Test:"$testTotal

		test_flags=$(echo " < "$test".og > "$test".out")
		../$runPath < $test.og > $test.out
		
		DIFF=$(diff -b $(echo $test".out") expected/$(echo $test".out"))
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
	done
	echo ""
	cd ..
done

echo "---------"
echo $testRight/$testTotal = $(echo $testRight*100 /$testTotal | bc)%
if [[ ! -z $unpassedTests ]]
then 
	echo -e $unpassedTests 
fi
echo "---------"

