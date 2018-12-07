MAKEFLAGS = --silent
all:
	echo Making AdvShell...
	make -C CircuitRouter-AdvShell/
	echo Done!
	echo Making SeqSolver...
	make -C CircuitRouter-SeqSolver/
	echo Done!

clean:	
	echo Cleaning .res, .old
	rm -f inputs/*.res inputs/*.old
	make clean -C CircuitRouter-AdvShell/
	make clean -C CircuitRouter-SeqSolver/
	echo Done!

clean_out:
	echo Cleaning .res, .old
	rm -f inputs/*.res inputs/*.old
	echo Done!

test:
	cd CircuitRouter-AdvShell/;\
	echo "run ../inputs/random-x128-y128-z3-n64.txt \n exit" > test.txt;\
	./CircuitRouter-AdvShell < test.txt;\
	rm test.txt;\
	cd ..
