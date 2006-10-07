export CFLAGS:=-I../ -g -Wall -pedantic-errors -std=gnu99
export LIBS:=-L../ -lDirt
export CC:=gcc
export LD_LIBRARY_PATH:=$(shell cd ..; pwd)

all: runBufferTest runWriteBufferTest runReaderTest runStructTest runStructReaderTest runWriterTest runStructWriterTest runStructReaderWriterTest

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

BufferTest: BufferTest.o
	$(CC) $(CFLAGS) $(LIBS) $< -o $@

WriteBufferTest: WriteBufferTest.o
	$(CC) $(CFLAGS) $(LIBS) $< -o $@

ReaderTest: ReaderTest.o
	$(CC) $(CFLAGS) $(LIBS) $< -o $@

StructTest: StructTest.o
	$(CC) $(CFLAGS) $(LIBS) $< -o $@

StructReaderTest: StructReaderTest.o
	$(CC) $(CFLAGS) $(LIBS) $< -o $@

WriterTest: WriterTest.o
	$(CC) $(CFLAGS) $(LIBS) $< -o $@

StructWriterTest: StructWriterTest.o
	$(CC) $(CFLAGS) $(LIBS) $< -o $@

StructReaderWriterTest: StructReaderWriterTest.o
	$(CC) $(CFLAGS) $(LIBS) $< -o $@

.PHONY: runBufferTest
.PHONY: runWriteBufferTest
.PHONY: runReaderTest
.PHONY: runStructTest
.PHONY: runStructReaderTest
.PHONY: runWriterTest
.PHONY: runStructWriterTest
.PHONY: runStructReaderWriterTest

runBufferTest: BufferTest
	./BufferTest

runWriteBufferTest: WriteBufferTest
	./WriteBufferTest

runReaderTest: ReaderTest
	./ReaderTest

runStructTest: StructTest
	./StructTest

runStructReaderTest: StructReaderTest
	./StructReaderTest > StructReaderTest.output
	diff ReaderTest.data StructReaderTest.output

runWriterTest: WriterTest
	./WriterTest
	chmod u+wr WriterTest.output
	diff WriterTest.data WriterTest.output

runStructWriterTest: StructWriterTest
	./StructWriterTest
	chmod u+wr StructWriterTest.output
	diff StructWriterTest.data StructWriterTest.output

runStructReaderWriterTest: StructReaderWriterTest
	./StructReaderWriterTest
	chmod u+wr StructReaderWriterTest.output
	diff StructReaderWriterTest.data StructReaderWriterTest.output

.PHONY: clean

clean:
	-rm -rf *Test *.output *.o *~ core*
