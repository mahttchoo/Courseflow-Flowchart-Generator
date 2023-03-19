CFLAGS = -std=c++14 -Wall -g

flowchart:	main.o coursenode.o lemon.o
	g++ main.o coursenode.o lemon.o -o flowchart $(CFLAGS)

main.o:	coursenode.h main.cpp
	g++ -c main.cpp $(CFLAGS)

coursenode.o: coursenode.h coursenode.cpp
	g++ -c coursenode.cpp $(CFLAGS)

lemon.o: lemon-1.3.1/lemon/smart_graph.h
	g++ -c lemon-1.3.1/lemon/smart_graph.h $(CFLAGS)

clean:
	rm -f *.o flowchart
