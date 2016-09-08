CC 	= g++ --std=c++11
CXXFLAG	= -pthread

streamGraphSample:streamGraphSample.o streamNS.o streamES.o piesN.o
	$(CC) $(CXXFLAG) -o streamGraphSample streamGraphSample.o streamNS.o streamES.o piesN.o

streamGraphSample.o:streamGraphSample.cpp streamGraphSample.h
	$(CC) $(CXXFLAG) -c streamGraphSample.cpp

streamNS.o:streamNS.cpp streamNS.h
	$(CC) $(CXXFLAG) -c streamNS.cpp

streamES.o:streamES.cpp streamES.h
	$(CC) $(CXXFLAG) -c streamES.cpp

piesN.o:piesN.cpp piesN.h
	$(CC) $(CXXFLAG) -c piesN.cpp

clean:
	rm streamGraphSample streamGraphSample.o streamNS.o streamES.o piesN.o

test:
	./streamGraphSample piesn testdata/rmat-15-8 6000 testdata/dataPIESN 1
	
