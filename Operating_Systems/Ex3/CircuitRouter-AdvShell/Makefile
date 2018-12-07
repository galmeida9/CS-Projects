# Makefile, versao 1
# Sistemas Operativos, DEI/IST/ULisboa 2018-19

SOURCES = CircuitRouter-AdvShell.c 
SOURCES+= ../lib/commandlinereader.c ../lib/vector.c
OBJS = $(SOURCES:%.c=%.o)
CC   = gcc
CFLAGS =-Wall -std=gnu99 -pthread -I../
LDFLAGS=-lm
TARGET = CircuitRouter-AdvShell

all: $(TARGET) 

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $(TARGET) $(LDFLAGS) 

CircuitRouter-AdvShell.o: CircuitRouter-AdvShell.c CircuitRouter-AdvShell.h ../lib/commandlinereader.h ../lib/vector.h
../lib/commandlinereader.o: ../lib/commandlinereader.c ../lib/commandlinereader.h
../lib/vector.o: ../lib/vector.c ../lib/vector.h ../lib/types.h ../lib/utility.h

$(OBJS):
	$(CC) $(CFLAGS) -c -o $@ $<
	$(CC) $(CFLAGS) -o CircuitRouter-Client CircuitRouter-Client.c

clean:
	@echo Cleaning AdvShell...
	rm -f $(OBJS) $(TARGET)
	rm -f CircuitRouter-Client 
  
