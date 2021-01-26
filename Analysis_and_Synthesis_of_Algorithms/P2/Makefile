MAKEFLAGS+=--silent
all:
	gcc -g -O3 -ansi -Wall src/p2.c -lm -o P2.out

run:
	./run-tests.sh P2.out

cpp:
	g++ -std=c++11 -O3 -Wall -lm -o P2.outc src/p2.cpp

clean:
	rm P2.out
