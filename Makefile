CC 	= g++ --std=c++11
CXXFLAG	= -pthread

streamGraphSample:streamGraphSample.o streamNS.o streamES.o
	$(CC) $(CXXFLAG) -o streamGraphSample streamGraphSample.o streamNS.o streamES.o

streamGraphSample.o:streamGraphSample.cpp streamGraphSample.h
	$(CC) $(CXXFLAG) -c streamGraphSample.cpp

streamNS.o:streamNS.cpp streamNS.h
	$(CC) $(CXXFLAG) -c streamNS.cpp

streamES.o:streamES.cpp streamES.h
	$(CC) $(CXXFLAG) -c streamES.cpp

clean:
	rm streamGraphSample streamGraphSample.o streamNS.o streamES.o

test:
	./streamGraphSample sns testdata/rmat-15-8 6000 testdata/dataSNS
	./streamGraphSample ses testdata/rmat-15-8 6000 testdata/dataSES
	
