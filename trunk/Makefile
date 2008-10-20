# Makefile

OBJECTS := dnssocket.o dnspacketizator.o dnsresolver.o dnsdatabasereader.o dnsmain.o

.PHONY : clean

dnsserver : $(OBJECTS)
	$(CXX) $(OBJECTS) -o dnsserver

dnsmain.o : dnsmain.cpp
	$(CXX) -c -I. -g dnsmain.cpp -o dnsmain.o

dnsdatabasereader.o : dnsdatabasereader.cpp dnsdatabasereader.h
	$(CXX) -c -I. -g dnsdatabasereader.cpp -o dnsdatabasereader.o

dnsresolver.o : dnsresolver.cpp dnsresolver.h
	$(CXX) -c -I. -g dnsresolver.cpp -o dnsresolver.o

dnspacketizator.o : dnspacketizator.cpp dnspacketizator.h
	$(CXX) -c -I. -g dnspacketizator.cpp -o dnspacketizator.o 

dnssocket.o : dnssocket.cpp dnssocket.h
	$(CXX) -c -I. -g dnssocket.cpp -o dnssocket.o

clean:
	rm -f *.o dnsserver

