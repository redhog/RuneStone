export CFLAGS:=-I../ -g -Wall -pedantic-errors -std=gnu99
export LIBS:=-L../ -lDirt -lRuneStone
export CC:=gcc
export LD_LIBRARY_PATH:=$(shell cd ..; pwd):$(LD_LIBRARY_PATH)

all: runObjectTest

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

ObjectTest: ObjectTest.o
	$(CC) $(CFLAGS) $(LIBS) $< -o $@

.PHONY: runObjectTest

runObjectTest: ObjectTest
	./ObjectTest
	chmod u+wr ObjectTest.output
	diff ObjectTest.data ObjectTest.output

.PHONY: clean

clean:
	-rm -rf *Test *.output *.o *~ core*
