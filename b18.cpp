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

void printArray(vector<nand> nandVector);

int main( int argc, char * argv[])
{
	ifstream wiringFile;
	const int j = 8; //Input Lines
	const int k = 4; //Output Lines
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

	inputPins.reserve(usedInputs.size());
	outputPins.reserve(usedOutputs.size());

	for(set<int>::iterator iter = usedInputs.begin(); iter != usedInputs.end(); iter++)
	{
		cout << *iter << " " << endl;
	}

	cout << "Used Inputs: " << usedInputs.size() << endl;
	cout << "Used Outputs: " << usedOutputs.size() << endl;

	//Create nand vector
	for(int i = 0; i < m * n; i++)
	{
		nand tempNand;
		nandVector.push_back(tempNand);
	}

	//Set up nandVector to have pointers to the correct things
	for(int i = 0; i < inputs.size(); i++)
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
			if(outputs[i] >= nandVector.size() * 2)
			{
				//outputPins[outputs[i] - (2 * m * n)] = &nandVector[inputs[i] - 8].output;
				outputPins.push_back(&nandVector[inputs[i] - 8].output);				
			}
			if(outputs[i] % 2)
				nandVector[floor(outputs[i] / 2)].input1 = &nandVector[inputs[i] - 8].output;
			else
				nandVector[floor(outputs[i] / 2)].input2 = &nandVector[inputs[i] - 8].output;
		}
	}

	

	//Binary Counter Thing
	for(int i = 0; i < pow(2, usedInputs.size()); i++)
	{
		for(int iter1 = usedInputs.size() - 1; iter1 >= 0; iter1--)
		{
			inputPins[iter1] = myBitset[iter1];
			cout << inputPins[iter1] << " ";
		}
		
		/*for(int iter1 = 0; iter1 < usedInputs.size(); iter1++)
		{
			inputPins[iter1] = myBitset[iter1];
			cout << inputPins[iter1] << " ";
		}*/

		cout << endl;

		for(int iter1 = 0; iter1 < nandVector.size(); iter1++)
		{
			nandVector[iter1].output = !(*nandVector[iter1].input1 & *nandVector[iter1].input2);
			//cout << "All Inputs1: " << *nandVector[i].input1 << endl;
			//cout << "All Inputs2: " << *nandVector[i].input2 << endl;
			//cout << "All Outputs" << iter1 << ": " << nandVector[i].output << endl;
		}		

		cout << "OutputsPins: ";
		for(int iter1 = 0; iter1 < outputPins.size(); iter1++)
		{
			cout << *outputPins[iter1] << " ";
		}
		cout << endl;
		/*
		cout << "Input1[0]: " << *nandVector[0].input1 << endl;
		cout << "Input2[0]: " << *nandVector[0].input2 << endl;
		cout << "Outputs[0]: " << nandVector[0].output << endl;

		cout << "Input1[1]: " << *nandVector[1].input1 << endl;
		cout << "Input2[1]: " << *nandVector[1].input2 << endl;
		cout << "Outputs[1]: " << nandVector[1].output << endl;

		cout << "Input1[13]: " << *nandVector[13].input1 << endl;
		cout << "Input2[13]: " << *nandVector[13].input2 << endl;
		cout << "Outputs[13]: " << nandVector[13].output << endl;
		*/

		//printArray(nandVector);
		cout << "DONE" << endl;

		myBitset = bitset<j>(myBitset.to_ulong() + 1);
	}	
}

void printArray(vector<nand> nandVector)
{
	for(int i = 0; i < nandVector.size(); i++)
	{
		if(i % 10 == 0 && i != 0)
		{
			cout << endl;
		}
		cout << nandVector[i].output << " ";
	}
	cout << endl;
}
