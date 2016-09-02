#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>

#include "streamNS.h"

using namespace std;

class streamGraphSample
{
public:
	streamGraphSample( string inputFile );
	~streamGraphSample(void);

	string inputFile;
	int sampleSize;
	string outputFile;

	void streamNS( string inputFile , int sampleSize , string outputFile );

};


