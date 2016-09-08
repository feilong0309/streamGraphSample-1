#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>

#include "streamNS.h"
#include "streamES.h"
#include "piesN.h"

using namespace std;

class streamGraphSample
{
public:
	streamGraphSample( string inputFile );
	~streamGraphSample(void);

	string inputFile;
	int sampleSize;
	string outputFile;
	float paraN;

	void streamNS( string inputFile , int sampleSize , string outputFile );
	void streamES( string inputFile , int sampleSize , string outputFile );
	void piesN( string inputFile , int sampleSize , string outputFile , float paraN );
};


