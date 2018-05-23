CPPFLAGS=-g -Wall -std=c++11
SRCDIR=src/

all: reader.bin clean

reader.bin: main.o
	g++ $(CPPFLAGS) main.o Chain.o Block.o Transaction.o Input.o Output.o -o reader.bin
	
main.o: Chain.o
	g++ $(CPPFLAGS) $(SRCDIR)main.cpp -c -o main.o
	 
Chain.o: Block.o
	g++ $(CPPFLAGS) $(SRCDIR)Chain.cpp -c -o Chain.o
	
Block.o: Transaction.o
	g++ $(CPPFLAGS) $(SRCDIR)Block.cpp -c -o Block.o
	
Transaction.o: Input.o Output.o
	g++ $(CPPFLAGS) $(SRCDIR)Transaction.cpp -c -o Transaction.o
	
Input.o:
	g++ $(CPPFLAGS) $(SRCDIR)Input.cpp -c -o Input.o
	
Output.o:
	g++ $(CPPFLAGS) $(SRCDIR)Output.cpp -c -o Output.o
	
clean:
	rm *.o

veryclean:
	rm *.o reader.bin
