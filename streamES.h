#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <time.h>
#include <stdlib.h>

using namespace std;

class streamES
{
public:
	streamES( string inputFile , int sampleSize , string outputFile );
	~streamES();

	string inputFile;
	int size;
	string outputFile;

	map<uint64_t,map<uint64_t,uint64_t>> EShash;//<边的hash值，<from,to>>
			  //考虑到可能有重复边，由于是无向图，所以要求from<to
	map<uint64_t,int> VSdegree;//<点ID，点的度>

	void process();
	uint64_t hash64( uint64_t , uint64_t , int );
	void delEdge();
	void saveSampleGraph();

};

	
