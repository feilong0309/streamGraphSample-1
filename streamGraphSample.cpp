#include "streamGraphSample.h"

int main( int argc , char* argv[] )
{
	if ( argc < 5 )
	{
		cout << "command method inputfile sampleSize outputFile" << endl;
		return 0;
	}
	string method(argv[1]); 
	string input(argv[2]);
	int sampleSize = atoi(argv[3]);
	string output(argv[4]);

	if ( method == "SNS" || method == "sns" )
	{
		streamNS myStreamNS( input , sampleSize , output );
		time_t t_start,t_end;
		t_start = time(NULL);
		myStreamNS.process();
		t_end = time(NULL);
		cout<<"time cost : "<<difftime(t_end,t_start)<<endl;
		myStreamNS.saveSampleGraph();
		return 0;
	}
	else
	{
		cout<<" command method inputfile sampleSize" <<endl;
		cout<<" streamNS : method = SNS || method = sns "<<endl;
		cout<<argv[0]<<"  "<<argv[1]<<"  "<<argv[2]<<"  "<<argv[3]<<"  "<<argv[4]<<endl;
		return 0;
	}
}

