#include "piesN.h"

piesN::piesN( string inFile , int sampleSize , string outFile , float n )
{
	inputFile = inFile;
	size = sampleSize;
	outputFile = outFile;
	paraN =n;
}

piesN::~piesN()
{
}

void piesN::process()
{
	ifstream ifs(inputFile);
	if( !ifs.is_open() )
	{
		cout<<"Error opening file";
		exit(1);
	}

	cout<<"sampling......"<<endl;
	string line;
	uint64_t from,to;

	time_t timep;
	int salt = time(&timep);

	int t = 1;
	float pe = 0;
	float r = 0;

	while( getline( ifs,line ))
	{
		srand( (unsigned)time(NULL) );
		
		sscanf(line.c_str(),"%ld %ld",&from,&to);

		if ( VSdegree.size() < size )
		{
			if( VSdegree.find(from) == VSdegree.end() )
			{
				VSdegree.insert( pair<uint64_t,int>(from,0) );
			}
			if( VSdegree.find(to) == VSdegree.end() )
			{
				VSdegree.insert( pair<uint64_t,int>(to,0) );
			}

			if( 1 )
			{
				ES.insert( pair<uint64_t,uint64_t>(from,to) );
				ES.insert( pair<uint64_t,uint64_t>(to,from) );

				VSdegree[from]++;
				VSdegree[to]++;

				updateDegreeMap(VSdegree[from],1);
				updateDegreeMap(VSdegree[to],1);
			}
		}
		else
		{
			pe = ES.size() / 2 / t;
			r = rand() / double(RAND_MAX);

			if ( r <= pe )
			{
				if ( VSdegree.find(from) == VSdegree.end() )
				{
					dropNode();
					VSdegree.insert(pair<uint64_t,int>(from,0));
				}
				if ( VSdegree.find(to) == VSdegree.end() )
				{
					dropNode();
					VSdegree.insert(pair<uint64_t,int>(to,0));
				}
			}

			if ( VSdegree.find(from) != VSdegree.end() && VSdegree.find(to) != VSdegree.end() )
			{
				ES.insert( pair<uint64_t,uint64_t>(from,to) );
				ES.insert( pair<uint64_t,uint64_t>(to,from) );

				VSdegree[from]++;
				VSdegree[to]++;

				updateDegreeMap(VSdegree[from],1);
				updateDegreeMap(VSdegree[to],1);
			}
		}

		t++;

	}
}

void piesN::updateDegreeMap( int degree , int para)
{
	if ( para > 0 )
	{
		if ( degree <= 0 ) return;

		if ( degreeMap.find(degree) == degreeMap.end() )
			degreeMap.insert( pair<int,int>(degree,0) );
	
		if ( degree == 1 )
			degreeMap[degree]++;
		else
		{
			degreeMap[degree] += para;
			degreeMap[degree-1] -= para;
		}
	}
	if ( para < 0 )
	{
		if ( degree <= 0 ) return;

		if ( degreeMap.find(degree) == degreeMap.end() )
			return;

		if ( degreeMap[degree] <= 0 )
			degreeMap.erase(degree);
		else
		{
			degreeMap[degree] += para;
		}
	}
}

void piesN::dropNode()
{
	int max = 0;
	map<int,int>::iterator it;
	for ( it = degreeMap.begin() ; it != degreeMap.end() ; it++ )
	{
		max = max + (float)1.0 / ( (it->first)*pow(it->second,paraN) );
	}
	
	srand( (unsigned)time(NULL) );	

	float randomValue = rand() % ( size + 1 );

	map<uint64_t,int>::iterator itVSdegree;
	int rmVertex;
	
	float addValue = 0;
	float tempValue = 0;

	for ( itVSdegree = VSdegree.begin() ; itVSdegree != VSdegree.end() ; itVSdegree++ )
	{
		if ( it->second == 0 ) break;

		tempValue = (float)1.0 / ( (it->first)*pow(it->second,paraN) );	 
		addValue += tempValue;

		if ( addValue >= randomValue )
		{
			rmVertex = itVSdegree -> first;
			updateDegreeMap( itVSdegree->second , -1 );
			VSdegree.erase( itVSdegree );
			delEdge( rmVertex );
			break;
		}
	}
}
void piesN::delEdge( int node )
{
	multimap<uint64_t,uint64_t>::iterator itES;
	map<uint64_t,int>::iterator itVSdegree;

	multimap<uint64_t,uint64_t>::iterator end;
	map<uint64_t,int>::iterator endVSdegree;

	multimap<uint64_t,uint64_t>::iterator position = ES.lower_bound(node);
	multimap<uint64_t,uint64_t>::iterator upper = ES.upper_bound(node);

	multimap<uint64_t,uint64_t>::iterator position2;
	multimap<uint64_t,uint64_t>::iterator upper2;

	while( position != upper )
	{
		position2 = ES.lower_bound(position->second);
		upper2 = ES.upper_bound(position->second);
		while( position2 != upper2 )
		{
			if ( position2->second == node )
			{
				itVSdegree = VSdegree.find( position2->first );
				itVSdegree->second -= 1;
				
				updateDegreeMap( itVSdegree->second , 1 );
				updateDegreeMap( itVSdegree->second+1 , -1 );

				if ( itVSdegree->second <= 0 )
				{
					VSdegree.erase( itVSdegree );
				}

				if ( position2 == upper )
				{
					upper = ES.erase(position2);
				}
				else
				{
					ES.erase(position2);
				}

				break;
			}
		position2++;
		}
	position++;
	}
	ES.erase(node);
}

void piesN::saveSampleGraph()
{
	cout<<"VS COUNT: "<<VSdegree.size()<<"  "<<" ES COUNT: "<<ES.size()<<endl;

	multimap<uint64_t,uint64_t>::const_iterator it;
	ofstream out(outputFile);
	if ( ! out.is_open() )
	{
		cout<<"Error opening file"<<endl;
		exit(1);
	}

	for ( it = ES.begin(); it != ES.end() ; it++ )
	{
		out<<it->first<<" "<<it->second<<endl;
	}

	out.close();
}
