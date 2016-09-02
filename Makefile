CC 	= g++ --std=c++11
CXXFLAG	= -pthread

streamGraphSample:streamGraphSample.o streamNS.o
	$(CC) $(CXXFLAG) -o streamGraphSample streamGraphSample.o streamNS.o

streamGraphSample.o:streamGraphSample.cpp streamGraphSample.h
	$(CC) $(CXXFLAG) -c streamGraphSample.cpp

streamNS.o:streamNS.cpp streamNS.h
	$(CC) $(CXXFLAG) -c streamNS.cpp

clean:
	rm streamGraphSample streamGraphSample.o streamNS.o

test:
	./streamGraphSample sns testdata/data 80000 testdata/data2
	
