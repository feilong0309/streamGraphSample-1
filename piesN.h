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

class piesN

{
public:
	piesN( string inputFile , int sampleSize , string outputFile , float paraN );
	~piesN(void);

	string inputFile;
	int size;
	string outputFile;
	float paraN;
	
	map<uint64_t,int> VSdegree;//点ID，点ID的度
	map<int,int> degreeMap;//度，子图中拥有该度的点的个数，便于计算被替换概率
	multimap<uint64_t,uint64_t> ES;


	void process();
	void updateDegreeMap( int , int );
	void dropNode();
	void delEdge( int );
	void saveSampleGraph();
};

