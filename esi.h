#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <vector>
#include <time.h>
#include <stdlib.h>

using namespace std;

class esi
{
public:
	esi( string inputFile , int sampleSize , string outputFile );
	~esi();

	string inputFile;
	int size;
	string outputFile;

	map<uint64_t,int> VShash;
	set<uint64_t> VS;
	map<uint64_t,uint64_t> ES;

	void process();
	uint64_t hash64( uint64_t , int );
	void saveSampleGraph();

};

	
