#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <math.h>

using namespace std;

class streamNS
{
public:
	streamNS( string inputFile , int sampleSize , string outputFile );
	~streamNS(void);

	string inputFile;
	int size;
	string outputFile;

	map<uint64_t,int> VShash; //点ID的hash值，点ID
	map<int,int> VSdegree;//点ID，点ID的度
	multimap<int,int> ES;

	void process();
	uint64_t hash64( int , int );
	void delEdge( int , int );
	void saveSampleGraph();
};


