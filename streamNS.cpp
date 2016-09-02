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
	int from,to;
	bool isfrom,isto;

	time_t timep;
	int salt = time(&timep)%10000;

	while( getline( ifs,line ))
	{
		sscanf(line.c_str(),"%d %d",&from,&to);
		
		uint64_t hashfrom=hash64(from,salt);
		uint64_t hashto=hash64(to,salt);
		uint64_t max=0;

		if ( VShash.size()<size-1 )
		{

			if ( VShash.find(hashfrom)==VShash.end() ) 
			{
				VShash.insert( pair<uint64_t,int>(hashfrom,from) );
				VSdegree.insert( pair<int,int>(from,0) );
			}
			if ( VShash.find(hashto)==VShash.end() )
			{
				VShash.insert( pair<uint64_t,int>(hashto,to) );
				VSdegree.insert( pair<int,int>(to,0) );
			}
			if ( 1 ) 
			{
				ES.insert( pair<int,int>(from,to) );
				ES.insert( pair<int,int>(to,from) );
				
				VSdegree[from]++;
				VSdegree[to]++;
			}
		}
		else
		{
			//map<uint64_t,int>::iterator iter;
			//iter = VShash.end();
			//iter --;
			//max=iter -> first;
			
			max=VShash.rbegin()->first;

			if ( hashfrom < max && VShash.find(hashfrom)==VShash.end() && VSdegree.find(from)==VSdegree.end() ) 
			{
				//cout<<"hashfrom="<<hashfrom<<"  "<<"max="<<max<<endl;

				VShash.insert( pair<uint64_t,int>(hashfrom,from) );
				VSdegree.insert( pair<int,int>(from,0) );

				//map<uint64_t,int>::iterator itmax;
				//map<int,int>::iterator itmax2;
				//itmax = VShash.find(max);
				//itmax2 = VSdegree.find( itmax->second );
				delEdge( VShash[max] , salt );
				if ( (VShash.find(max) != VShash.end()) && (VSdegree.find(VShash[max]) != VSdegree.end()) )
				{
					//delEdge( VShash[max] , salt );
					VSdegree.erase(VShash[max]);
					VShash.erase(max);
				}
			}

			//iter = VShash.end();
			//iter --;
			//max=iter -> first;
			
			max=VShash.rbegin()->first;

			if ( hashto < max && max != hashfrom && VShash.find(hashto)==VShash.end() && VSdegree.find(to)==VSdegree.end() )
			{
				//cout<<"hashto="<<hashto<<"  "<<"max="<<max<<endl;

				VShash.insert( pair<uint64_t,int>(hashto,to) );
				VSdegree.insert( pair<int,int>(to,0) );

				//map<uint64_t,int>::iterator itmax;
				//map<int,int>::iterator itmax2;
				//itmax = VShash.find(max);
				//itmax2 = VSdegree.find( itmax->second );
				delEdge( VShash[max] , salt );
				if ( (VShash.find(max) != VShash.end()) && (VSdegree.find(VShash[max]) !=VSdegree.end()) ) 
				{
					//delEdge( VShash[max] , salt );
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

uint64_t streamNS::hash64(int a , int salt)
{
 	a = (~a) + (a << 21) + salt; // a = (a << 21) - a - 1 + salt;
	a = a ^ (a >> 24);
	a = (a + (a << 3)) + (a << 8); // a * 265
	a = a ^ (a >> 14);
	a = (a + (a << 2)) + (a << 4); // a * 21
	a = a ^ (a >> 28);
	a = a + (a << 31);
	return abs(a);
}

void streamNS::delEdge( int node ,int salt )
{
	multimap<int,int>::iterator itES;
	map<int,int>::iterator itVSdegree;

	multimap<int,int>::iterator end;
	map<int,int>::iterator endVSdegree;

	multimap<int,int>::iterator position = ES.lower_bound(node);
	multimap<int,int>::iterator upper = ES.upper_bound(node);

	multimap<int,int>::iterator position2;
	multimap<int,int>::iterator upper2;

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
					map<uint64_t,int>::iterator temp;
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

	multimap<int,int>::const_iterator it;
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
		
