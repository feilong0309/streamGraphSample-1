#include "streamNS.h"

streamNS::streamNS(string inFile,int sampleSize , string outFile )
{
	inputFile=inFile;
	size=sampleSize;
	outputFile=outFile;
}

streamNS::~streamNS()
{
}

void streamNS::process()
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
	bool isfrom,isto;

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
				VSdegree.insert( pair<uint64_t,int>(from,0) );
			}
			if ( VShash.find(hashto)==VShash.end() )
			{
				VShash.insert( pair<uint64_t,uint64_t>(hashto,to) );
				VSdegree.insert( pair<uint64_t,int>(to,0) );
			}
			if ( 1 ) 
			{
				ES.insert( pair<uint64_t,uint64_t>(from,to) );
				ES.insert( pair<uint64_t,uint64_t>(to,from) );
				
				VSdegree[from]++;
				VSdegree[to]++;
			}
		}
		else
		{	
			max=VShash.rbegin()->first;

			if ( hashfrom < max && VShash.find(hashfrom)==VShash.end() && VSdegree.find(from)==VSdegree.end() ) 
			{

				VShash.insert( pair<uint64_t,uint64_t>(hashfrom,from) );
				VSdegree.insert( pair<uint64_t,int>(from,0) );

				delEdge( VShash[max] , salt );

				if ( (VShash.find(max) != VShash.end()) && (VSdegree.find(VShash[max]) != VSdegree.end()) )
				{
					VSdegree.erase(VShash[max]);
					VShash.erase(max);
				}
			}

			
			max=VShash.rbegin()->first;

			if ( hashto<max && max!=hashfrom && VShash.find(hashto)==VShash.end() && VSdegree.find(to)==VSdegree.end() )
			{
				VShash.insert( pair<uint64_t,uint64_t>(hashto,to) );
				VSdegree.insert( pair<uint64_t,int>(to,0) );

				delEdge( VShash[max] , salt );

				if ( (VShash.find(max) != VShash.end()) && (VSdegree.find(VShash[max]) !=VSdegree.end()) ) 
				{
					VSdegree.erase(VShash[max]);
					VShash.erase(max);
				}
			}
			
			
			if ( (VShash.find(hashfrom)!=VShash.end()) && (VShash.find(hashto)!=VShash.end()) )
			{
				ES.insert( pair<int,int>(from,to) );
			  	ES.insert( pair<int,int>(to,from) );

				VSdegree[from]++;
				VSdegree[to]++;
			}

		}
		//cout<<"a edge process is finished and max = "<<VShash.rbegin()->first<<endl;
	}
}

uint64_t streamNS::hash64(uint64_t a , int salt)
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

void streamNS::delEdge( uint64_t node ,int salt )
{
	multimap<uint64_t,uint64_t>::iterator itES;
	map<uint64_t,int>::iterator itVSdegree;

	multimap<uint64_t,uint64_t>::iterator end;
	map<uint64_t,int>::iterator endVSdegree;

	multimap<uint64_t,uint64_t>::iterator position = ES.lower_bound(node);
	multimap<uint64_t,uint64_t>::iterator upper = ES.upper_bound(node);

	multimap<uint64_t,uint64_t>::iterator position2;
	multimap<uint64_t,uint64_t>::iterator upper2;

	while ( position != upper )
	{
		position2=ES.lower_bound( position->second );
		upper2=ES.upper_bound( position->second );
		while ( position2 != upper2 )
		{
			if ( position2->second == node )
			{
				itVSdegree = VSdegree.find( position2->first );
				itVSdegree->second -= 1;

				if ( itVSdegree->second <= 0 )
				{
					map<uint64_t,uint64_t>::iterator temp;
					temp = VShash.find( hash64(itVSdegree->first,salt) );
					if ( temp!= VShash.end() )
					{
						VShash.erase(temp);
						VSdegree.erase( itVSdegree );
					}
				}

				if ( position2 == upper )
				{
					upper = ES.erase( position2 );
				}
				else
				{
					ES.erase( position2 );
				}
				break;
			}
			++position2;
		}
		position++;
	}
	ES.erase( node );
	
	//cout<<"node "<< node << " is deleted and max ="<< VShash.rbegin()->first <<endl;
}
	
void streamNS::saveSampleGraph()
{
	cout<<"VS COUNT: "<<VSdegree.size()<<"  "<<VShash.size()<<"  "<<" ES COUNT: "<<ES.size()<<endl;

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
		
