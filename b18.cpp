#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <bitset>
#include <cmath>

using namespace std;

struct nand
{
	int input1;
	int input2;
	int output;
};

int main( int argc, char * argv[])
{
	ifstream wiringFile;
	const int j = 8; //Input Lines
	const int k = 4; //Output Lines
	const int m = 10; //Rows
	const int n = 10; //Columns
	vector<int> inputs;
	vector<int> outputs;
	set<int> usedInputs;
	set<int> usedOutputs;
	bitset<j> myBitset;

	cout << "Arg: " << argv[1] << endl;

	wiringFile.open(argv[1]);

	if(!wiringFile)
	{
		cout << "Wiring file failed to open" << endl;
		return 1;
	}

	int temp;
	int counter = 0;
	while(wiringFile >> temp)
	{
		if(temp < j)
			usedInputs.insert(temp);
		if(temp >= m * n * 2)
			usedOutputs.insert(temp);
		
		if(counter % 2 == 0)
		{
			inputs.push_back(temp);
			counter++;
		}
		else
		{
			outputs.push_back(temp);	
			counter++;
		}
	}

	cout << "Inputs: " << endl;	

	for(int i = 0; i < inputs.size(); i++)
	{
		cout << inputs[i] << endl;
	}

	cout << "Outputs: " <<  endl;

	for(int i = 0; i < outputs.size(); i++)
	{
		cout << outputs[i] << endl;
	}
	cout << "usedInputs: " << usedInputs.size() << endl;
	cout << "usedOutputs: " << usedOutputs.size() << endl;
	for(int i = 0; i < pow(2, usedInputs.size()); i++)
	{
		for(int iter1 = usedInputs.size() - 1; iter1 >= 0; iter1--)
		{
			cout << myBitset[iter1] << " ";
		}
		cout << endl;
		myBitset = bitset<j>(myBitset.to_ulong() + 1);
	}
}

void readWiring()
{
	
}
