# Makefile

#CC := g++
#CFLAGS := -O2
OBJECTS := dnssocket.o dnspacketizator.o dnsresolver.o dnsmain.o

.PHONY : clean

dnsserver : $(OBJECTS)
	g++ $(OBJECTS) -o dnsserver

dnsmain.o : dnsmain.cpp
	g++ -c -I. dnsmain.cpp -o dnsmain.o

dnsresolver.o : dnsresolver.cpp dnsresolver.h
	g++ -c -I. dnsresolver.cpp -o dnsresolver.o

dnspacketizator.o : dnspacketizator.cpp dnspacketizator.h
	g++ -c -I. dnspacketizator.cpp -o dnspacketizator.o 

dnssocket.o : dnssocket.cpp dnssocket.h
	g++ -c -I. dnssocket.cpp -o dnssocket.o

clean:
	rm -f *.o

