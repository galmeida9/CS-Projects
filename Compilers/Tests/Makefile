#MAKEFLAGS += --silent
path=../og
runPath=$(path)/og
folder=T-auto-tests/
test=V-02-138-N-ok
test_flags=

all:
	./allLogged.sh $(path)

clean:
	rm -f $(folder)*.out

cleanLog:
	rm -r Logs

run:
	$(runPath) --target asm < $(folder)$(test).og > $(test).asm
	yasm -felf32 $(test).asm
	ld -m elf_i386 -o $(test) $(test).o -L/home/daniel/compiladores/root/usr/lib/ -lrts
	./$(test) > $(test).out
	rm $(test).asm $(test).o $(test)
	diff -wb $(test).out $(folder)expected/$(test).out 

post:
	$(runPath) -g --target asm < $(folder)$(test).og | sed -n '/; /s/; //p'

show:
	@cat $(folder)$(test).og

xml:
	$(runPath) -g --tree < $(folder)$(test).og 
	

