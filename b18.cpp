#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <bitset>
#include <cmath>

using namespace std;

struct nand
{
	int* input1;
	int* input2;
	int output;
	int defaultInput;

	nand()
	{
		defaultInput = 1;
		input1 = &defaultInput;
		input2 = &defaultInput;
	}
};

void processAndPrint();
void nandSetup(vector<nand>& nandVector, vector<int>& inputs, vector<int>& outputs, vector<int>& inputPins,
			vector<int*>& outputPins, int j);
void printArray(vector<nand> nandVector);

int main( int argc, char * argv[])
{
	ifstream wiringFile;
	const int j = 8; //Input Lines
	//const int k = 4; //Output Lines
	const int m = 10; //Rows
	const int n = 10; //Columns
	vector<int> inputs;
	vector<int> outputs;
	vector<int> inputPins;
	vector<int*> outputPins;
	set<int> usedInputs;
	set<int> usedOutputs;
	bitset<j> myBitset;
	vector<nand> nandVector;

	wiringFile.open(argv[1]);

	if(!wiringFile)
	{
		cout << "Wiring file failed to open" << endl;
		return 1;
	}

	int temp;
	int counter = 0;
	//Read in file
	while(wiringFile >> temp)
	{		
		if(counter % 2 == 0)
		{
			if(temp < j)
				usedInputs.insert(temp);

			inputs.push_back(temp);
			counter++;
		}
		else
		{
			if(temp >= m * n * 2)
				usedOutputs.insert(temp);

			outputs.push_back(temp);	
			counter++;
		}
	}

	//These two lines are to make sure our vectors have enough space
	//so we can index into them
	inputPins.reserve(usedInputs.size());
	outputPins.reserve(usedOutputs.size());

	//Create nand vector
	for(unsigned int i = 0; i < m * n; i++)
	{
		nand tempNand;
		nandVector.push_back(tempNand);
	}	

	//Connects nand gates together
	nandSetup(nandVector, inputs, outputs, inputPins, outputPins, j);

	//Binary Counter Thing
	for(unsigned int i = 0; i < pow(2, usedInputs.size()); i++)
	{
		cout << "  ";
		for(unsigned int iter1 = 0; iter1 < usedInputs.size(); iter1++)
		{
			cout << iter1 << " ";
		}	
		cout << "  ";
		for(unsigned int iter1 = 0; iter1 < usedOutputs.size(); iter1++)
		{
			cout << (iter1 + m * n * 2) << " ";
		}
		cout << endl;

		int dashes = usedInputs.size() + usedOutputs.size() + 4;
		dashes *= 2;
		for(int iter1 = 0; iter1 < dashes; iter1++)
		{
			cout << "-";
		}
		cout << endl;


		cout << "| ";
		//Input Pins
		for(int iter1 = usedInputs.size() - 1; iter1 >= 0; iter1--)
		{
			inputPins[iter1] = myBitset[iter1];
			cout << inputPins[iter1] << " ";
		}

		cout << "|  ";		

		for(unsigned int iter1 = 0; iter1 < nandVector.size(); iter1++)
		{
			nandVector[iter1].output = !(*nandVector[iter1].input1 & *nandVector[iter1].input2);
		}		

		//Output Pins
		for(unsigned int iter1 = 0; iter1 < outputPins.size(); iter1++)
		{
			cout << *outputPins[iter1] << "  ";
		}
		cout << "|" << endl; 

		for(int iter1 = 0; iter1 < dashes; iter1++)
		{
			cout << "-";
		}
		cout << endl << endl;

		myBitset = bitset<j>(myBitset.to_ulong() + 1);
	}	
}

void processAndPrint()
{

}

void nandSetup(vector<nand>& nandVector, vector<int>& inputs, vector<int>& outputs, vector<int>& inputPins,
			vector<int*>& outputPins, int j)
{
	//Set up nandVector to have pointers to the correct nand gates
	for(unsigned int i = 0; i < inputs.size(); i++)
	{
		if(inputs[i] < j)
		{
			if(outputs[i] % 2)
				nandVector[floor(outputs[i] / 2)].input1 = &inputPins[inputs[i]];
			else
				nandVector[floor(outputs[i] / 2)].input2 = &inputPins[inputs[i]];
		}
		else
		{
			if((unsigned)outputs[i] >= nandVector.size() * 2)
			{				
				outputPins.push_back(&nandVector[inputs[i] - 8].output);				
			}
			if(outputs[i] % 2)
			{
				nandVector[floor(outputs[i] / 2)].input1 = &nandVector[inputs[i] - 8].output;
			}
			else
			{
				nandVector[floor(outputs[i] / 2)].input2 = &nandVector[inputs[i] - 8].output;
			}
		}		
	}
}
