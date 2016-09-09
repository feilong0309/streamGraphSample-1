#include "esi.h"

esi::esi(string inFile,int sampleSize , string outFile )
{
	inputFile=inFile;
	size=sampleSize;
	outputFile=outFile;
}

esi::~esi()
{
}

void esi::process()
{
	ifstream ifs(inputFile);
	if( !ifs.is_open() )
	{
		cout<<"Error opening file";
		exit(1);
	}

	cout<<"sampling......"<<endl;
	string line;
	uint64_t from = 0;
	uint64_t to = 0;
	
	time_t timep;
	int salt = time(&timep);

	while( getline( ifs,line ))
	{
		sscanf(line.c_str(),"%ld %ld",&from,&to);
		
		uint64_t hashfrom=hash64(from,salt);
		uint64_t hashto=hash64(to,salt);
		uint64_t max=0;

		if ( VShash.size() < size )
		{

			if ( VShash.find(hashfrom)==VShash.end() ) 
			{
				VShash.insert( pair<uint64_t,uint64_t>(hashfrom,from) );
				VS.insert(from);
			}
			if ( VShash.find(hashto)==VShash.end() )
			{
				VShash.insert( pair<uint64_t,uint64_t>(hashto,to) );
				VS.insert(to);
			}
		}
		else
		{	
			max=VShash.rbegin()->first;

			if ( hashfrom < max && VShash.find(hashfrom)==VShash.end() && VS.find(from)==VS.end() ) 
			{

				VShash.insert( pair<uint64_t,uint64_t>(hashfrom,from) );
				VS.insert(from);


				if ( (VShash.find(max) != VShash.end()) && (VS.find(VShash[max]) != VS.end()) )
				{
					VS.erase(VShash[max]);
					VShash.erase(max);
				}
			}

			
			max=VShash.rbegin()->first;

			if ( hashto<max && max!=hashfrom && VShash.find(hashto)==VShash.end() && VS.find(to)==VS.end() )
			{
				VShash.insert( pair<uint64_t,uint64_t>(hashto,to) );
				VS.insert(to);


				if ( (VShash.find(max) != VShash.end()) && (VS.find(VShash[max]) !=VS.end()) ) 
				{
					VS.erase(VShash[max]);
					VShash.erase(max);
				}
			}

		}
	}
	
	ifs.close();

	ifstream ifs2(inputFile);
	string line2;

	while ( getline( ifs2,line2 ) )
	{
		sscanf(line2.c_str(),"%ld %ld",&from,&to);

		if ( VS.find(from) != VS.end() && VS.find(to) != VS.end() )
			ES.insert(pair<uint64_t,uint64_t>(from,to));
	}
	
}

uint64_t esi::hash64(uint64_t a , int salt)
{
 	a = (~a) + (a << 21) + salt; // a = (a << 21) - a - 1 + salt;
	a = a ^ (a >> 24);
	a = (a + (a << 3)) + (a << 8); // a * 265
	a = a ^ (a >> 14);
	a = (a + (a << 2)) + (a << 4); // a * 21
	a = a ^ (a >> 28);
	a = a + (a << 31);
	return a;
}

void esi::saveSampleGraph()
{
	cout<<"VS COUNT: "<<VS.size()<<"  "<<VShash.size()<<"  "<<" ES COUNT: "<<ES.size()<<endl;

	multimap<uint64_t,uint64_t>::const_iterator it;
	ofstream out(outputFile);
	if ( ! out.is_open() )
	{
		cout << "Error opening file"<<endl;
		exit(1);
	}

	for ( it = ES.begin(); it !=ES.end() ; it++ )
	{
		out<<it->first<<" "<<it->second<<endl;
	}

	out.close();
}
		
