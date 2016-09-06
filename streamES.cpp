#include "streamES.h"

streamES::streamES( string inFile , int sampleSize , string outFile )
{
	inputFile=inFile;
	size=sampleSize;
	outputFile=outFile;
}

streamES::~streamES()
{
}

void streamES::process()
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
	uint64_t edgeHash;
	uint64_t max=0;

	time_t timep;
	int salt = time(&timep);

	while ( getline( ifs,line ) )
	{
		sscanf(line.c_str(),"%ld %ld",&from,&to);
		edgeHash=hash64( from , to , salt);
		
		if ( VSdegree.size()<size )
		{
			if ( EShash.find(edgeHash) == EShash.end() )
			{
				map<uint64_t,uint64_t> edge;
				edge.insert(pair<uint64_t,uint64_t>(from,to));
				EShash.insert(pair<uint64_t,map<uint64_t,uint64_t>>(edgeHash,edge));

				if( VSdegree.find(from) == VSdegree.end() )
					VSdegree.insert( pair<uint64_t,uint64_t>(from,1) );
				else
					VSdegree[from]++;

				if( VSdegree.find(to) == VSdegree.end() )
					VSdegree.insert( pair<uint64_t,uint64_t>(to,1) );
				else
					VSdegree[to]++;
			}
		}
		else
		{
			//cout << EShash.size()<<"  "<<VSdegree.size()<<endl;

			max = EShash.rbegin()->first;

			if( edgeHash < max && EShash.find(edgeHash) == EShash.end() )
			{
				map<uint64_t,uint64_t> edge;
				edge.insert(pair<uint64_t,uint64_t>(from,to));	
				EShash.insert(pair<uint64_t,map<uint64_t,uint64_t>>(edgeHash,edge));

				if( VSdegree.find(from) == VSdegree.end() )
					VSdegree.insert( pair<uint64_t,uint64_t>(from,1) );
				else
					VSdegree[from]++;

				if( VSdegree.find(to) == VSdegree.end() )
					VSdegree.insert( pair<uint64_t,uint64_t>(to,1) );
				else    
					VSdegree[to]++;

				delEdge();
			}
		}
	}
}

uint64_t streamES::hash64( uint64_t a , uint64_t b , int salt )
{
	a = (~a) + ( a << 21 ) + salt;
	b = (~a) + ( a << 21 ) + salt;

	a = a ^ ( a >> 24 );
	b = b ^ ( b >> 24 );

	a = ( a + ( a << 3 ) ) + ( a << 8 );
	b = ( b + ( b << 3 ) ) + ( b << 8 );

	a = a ^ ( a >> 14 );
	b = b ^ ( b >> 14 );

	a = ( a + ( a << 2 ) ) + ( a << 4 );
	b = ( b + ( b << 2 ) ) + ( b << 4 );

	a = a ^ ( a >> 28 );
	b = b ^ ( b >> 28 );

	a = a + ( a << 31 );
	b = b + ( b << 31 );

	return a+b;
}

void streamES::delEdge()
{
	uint64_t max=0;	
	uint64_t from,to = 0;
	
	cout<<EShash.size()<<"  "<<VSdegree.size()<<endl;

	while ( VSdegree.size() > size )
	{
		max = EShash.rbegin()->first;
		if ( EShash.find(max) != EShash.end() )
		{
			from = EShash[max].begin() -> first;
			to = EShash[max].begin() -> second;

			EShash.erase( max );

			if ( VSdegree.find(from) != VSdegree.end() )
				(VSdegree[from]>1) ? (VSdegree[from]--) : (VSdegree.erase(from));

			if ( VSdegree.find(to) != VSdegree.end() )
				(VSdegree[to]>1) ? ( VSdegree[to]--) : (VSdegree.erase(to));
		}
	}

	cout<<EShash.size()<<"  "<<VSdegree.size()<<endl;

}

void streamES::saveSampleGraph()
{
	cout<<"VS COUNT: "<<VSdegree.size()<<" ES COUNT: "<<EShash.size()<<endl;

	map<uint64_t,map<uint64_t,uint64_t>>::const_iterator it;
	ofstream out(outputFile);
	if ( ! out.is_open() )
	{
		cout << "Error opening file"<<endl;
		exit(1);
	}

	for ( it = EShash.begin(); it !=EShash.end() ; it++ )
	{
		out<<it->second.begin()->first<<" "<<it->second.begin()->second<<endl;
	}

	out.close();

}
		
