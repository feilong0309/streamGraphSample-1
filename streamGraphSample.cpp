#include "streamGraphSample.h"

int main( int argc , char* argv[] )
{
	if ( argc < 5 )
	{
		cout << "command method inputfile sampleSize outputFile paraN(only for piesN)" << endl;
		return 0;
	}
	string method(argv[1]); 
	string input(argv[2]);
	int sampleSize = atoi(argv[3]);
	string output(argv[4]);
	float paraN = 1;
	if ( argc = 6 )	  paraN = atof(argv[5]);

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
	else if ( method == "SES" || method == "ses" )
	{
		streamES myStreamES( input , sampleSize , output );
		time_t t_start,t_end;
		t_start = time(NULL);
		myStreamES.process();
		t_end = time(NULL);
		cout<<"time cost : "<<difftime(t_end,t_start)<<endl;
		myStreamES.saveSampleGraph();
		return 0;
	}
	else if ( method == "PIESN" || method == "piesN" || method == "piesn" )
	{
		if ( argc < 6 )
		{
			cout << "for piesN , parameter n is needed!!!"<<endl;
			cout << "command method inputfile sampleSize outputFile paraN" << endl;
			return 0;
		}
		piesN myPiesN( input , sampleSize , output , paraN );
		time_t t_start,t_end;
		t_start = time(NULL);
		myPiesN.process();
		t_end = time(NULL);
		cout<<"time cost : "<<difftime(t_end,t_start)<<endl;
		myPiesN.saveSampleGraph();
		return 0;
	}
	else
	{
		cout<<" command method inputfile sampleSize outputfile paraN(only for piesN)" <<endl;
		cout<<" streamNS : method = SNS || method = sns "<<endl;
		cout<<" streamES : method = SES || method = ses "<<endl;
		cout<<" piesN : method = PIESN || method = piesN || method = piesn "<<endl;
		return 0;
	}
}

