#include <vector>
#include <iostream>
#include <fstream>
#include <time.h>

using namespace std;

int num_run;
int num_candicate_point;
string output;

int main(int argc, char** argv)
{
	if (argc != 4)
	{
		cout << "ERROR: Does not match the number of parameters." << endl;
		exit(0);
	}
	
	srand(time(0));
	output = argv[1];
	num_run = atoi(argv[2]);
	num_candicate_point = atoi(argv[3]);
	
	
	ofstream fout;
	fout.open(output.c_str());
	for(int i = 0; i < num_run; i++)
	{
		for(int j = 0; j < num_candicate_point; j++)
			fout << (int)(rand() % 5) + 1 << ",";
		fout << endl;
	}
	fout.close(); 
	
	return 0;
}
