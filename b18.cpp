/*
 * Author: Christian Sieh
 * Program: B18
 * Instructor: Dr. Karlsson
 * Class: CSC317 Spring 2016
 * Last Modified: 3/23/2016
 *
 * Usage: b18 wiringFile.txt
 *
 * Description: 
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <bitset>
#include <cmath>

using namespace std;

const int j = 8; //Input Lines
//Output lines isn't used in my program. It gets the total output lines
//fromt he wiring diagram.
//const int k = 4; //Output Lines
const int m = 10; //Rows
const int n = 10; //Columns

//This is a structure to represent a NAND gate.
//The inputs are pointers so the can point to either input pins
//or they can point to the outputs of other NAND gates.
//defaultInput is just used to set the inputs for the NAND gates
//to 1.
struct nand
{
	int* input1;
	int* input2;
	int output;
	int defaultInput;
	
	//Constructor for the nand
	nand()
	{
		defaultInput = 1;
		input1 = &defaultInput;
		input2 = &defaultInput;
	}
};

void processAndPrint(int m, int n, set<int>& usedInputs, set<int>& usedOutputs, vector<int>& inputPins,
			vector<int*>& outputPins, vector<nand>& nandVector);
void nandSetup(vector<nand>& nandVector, vector<int>& inputs, vector<int>& outputs, vector<int>& inputPins,
			vector<int*>& outputPins, int j);
void printArray(vector<nand> nandVector);


/*
 * Author: Christian Sieh
 * 
 * Descripton: The main function for this program opens the input file specified in arg[1],
 * read in that file, and fill the nandVector with nand structures. Finally, it calls
 * nandSetup() to connect the grid together and processAndPrint to set the outputs of every
 * NAND gate and print the output to the screen.
 */ 
int main( int argc, char * argv[])
{
	ifstream wiringFile;
	vector<int> inputs;
	vector<int> outputs;
	vector<int> inputPins;
	vector<int*> outputPins;
	set<int> usedInputs;
	set<int> usedOutputs;
	vector<nand> nandVector;

	wiringFile.open(argv[1]);

	//If the file wasn't able to open
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
		//If the number is from the first column of the file
		//and is then an input else it's from the second column
		//and is an output
		if(counter % 2 == 0)
		{
			//If temp is less then j it is an inputPin so we
			//can add it to usedInputs so we can figure out
			//how many input pins we used
			if(temp < j)
				usedInputs.insert(temp);

			inputs.push_back(temp);
			counter++;
		}
		else
		{
			//If temp is greater than m * n * 2 then it is an
			//ouput and we add it to usedOutputs
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

	//Run all of the NAND gates to get their outputs and print to the console
	processAndPrint(m, n, usedInputs, usedOutputs, inputPins, outputPins, nandVector);
}

/*
 * Author: Christian Sieh
 *
 * Description: This general purpose of this function is to increment through a binary counter from
 * 0 - usedInputs^2. It uses the numbers in bitset for the input pins. Once the bitset is set then
 * it loops through every NAND gate and computes it's output. All of the other code in this function
 * deals with outputting to the console. 
 */ 
void processAndPrint(int m, int n, set<int>& usedInputs, set<int>& usedOutputs, vector<int>& inputPins,
			vector<int*>& outputPins, vector<nand>& nandVector)
{
	//Bitset that is used as our binary counter instead of using bitwise
	//operatiors and masking. This was something new to try. I would recommend
	//not doing it this way.
	bitset<j> myBitset;

	//This for loop goes through each nand gate and sets their outputs as
	//well as handling printing to the screen
	for(unsigned int i = 0; i < pow(2, usedInputs.size()); i++)
	{
		//Print the used inputs
		cout << "  ";
		for(unsigned int iter1 = 0; iter1 < usedInputs.size(); iter1++)
		{
			cout << iter1 << " ";
		}	
		cout << "  ";

		//Print the used Outputs
		for(unsigned int iter1 = 0; iter1 < usedOutputs.size(); iter1++)
		{
			cout << (iter1 + m * n * 2) << " ";
		}
		cout << endl;

		//Print out a line of dashes
		int dashes = usedInputs.size() + usedOutputs.size() + 4;
		dashes *= 2;
		for(int iter1 = 0; iter1 < dashes; iter1++)
		{
			cout << "-";
		}
		cout << endl;


		cout << "| ";
	
		//Print Input Pins
		for(int iter1 = usedInputs.size() - 1; iter1 >= 0; iter1--)
		{
			inputPins[iter1] = myBitset[iter1];
			cout << inputPins[iter1] << " ";
		}

		cout << "|  ";		

		//Set the outputs for all the nand gates
		for(unsigned int iter1 = 0; iter1 < nandVector.size(); iter1++)
		{
			nandVector[iter1].output = !(*nandVector[iter1].input1 & *nandVector[iter1].input2);
		}		

		//Print Output Pins
		for(unsigned int iter1 = 0; iter1 < outputPins.size(); iter1++)
		{
			cout << *outputPins[iter1] << "  ";
		}
		cout << "|" << endl; 

		//Print out the bottom line of dashes
		for(int iter1 = 0; iter1 < dashes; iter1++)
		{
			cout << "-";
		}
		cout << endl << endl;

		//Increment the counter
		myBitset = bitset<j>(myBitset.to_ulong() + 1);
	}	
}

/*
 * Author: Christian Sieh
 *
 * Description: The overall purpose of this funciton is to set the input1 and input2 pointers to point
 * to where they need to as well as the ponters in outputPins. This function goes through each input 
 * in the inputs vector and sets the correspongding value in the output vector to that input. If
 * the value is over the size of the nandVector then it is an output pin and we set the output pins
 * pointer to the output of the specified NAND gate.
 */ 
void nandSetup(vector<nand>& nandVector, vector<int>& inputs, vector<int>& outputs, vector<int>& inputPins,
			vector<int*>& outputPins, int j)
{
	//Set up nandVector to have pointers to the correct nand gates
	for(unsigned int i = 0; i < inputs.size(); i++)
	{
		//If the input is less than j it is an input pin so we need to point our NAND gate's inputs
		//towards an inputPin
		if(inputs[i] < j)
		{
			//If even or odd changes what input we set
			if(outputs[i] % 2)
				nandVector[floor(outputs[i] / 2)].input1 = &inputPins[inputs[i]];
			else
				nandVector[floor(outputs[i] / 2)].input2 = &inputPins[inputs[i]];
		}
		//Else if the it is not an input pin then we need to point our NAND gate's inputs towards
		//the outputs of another NAND gates
		else
		{
			//If greater than nandVecotr.size() then it is an output pin that we are setting
			if((unsigned)outputs[i] >= nandVector.size() * 2)
			{				
				outputPins.push_back(&nandVector[inputs[i] - 8].output);				
			}
			//if even or odd changes what input we set
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
