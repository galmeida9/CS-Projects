#!/bin/bash
path=$1
runPath=$path/og
testRight=0
testTotal=0
unpassedTests=""

cd $2
for f in $(ls *.og)	
do
	test=$(echo ${f::-3})
	echo /--------
	echo $test
	echo "Test:"$testTotal


	test_flags=$(echo " < "$test".og > "$test".out")
	../$runPath -g --target asm < $test.og > $test.asm
	yasm -felf32 $test.asm
	ld -melf_i386 -o $test $test.o -L/home/daniel/compiladores/root/usr/lib/ -lrts
	./$test > $test.out
	rm $test.asm $test.o $test
	DIFF=$(diff -b -w $(echo $test".out") expected/$(echo $test".out"))
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
	echo
done

echo $testRight/$testTotal = $(echo $testRight*100 /$testTotal | bc)%
echo -e $unpassedTests
