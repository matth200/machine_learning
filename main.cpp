#include <iostream>
#include <fstream>

#include <cstdlib>
#include <ctime>

#include <string.h>
#include "m_learning.h"

#define LEARNING_RATE 0.4

using namespace std;
int main(int argc, char *argv[])
{
	srand(time(0));

	cout << "initialisation" << endl;
	char data[784];
	
	MachineLearning machine(784);
	//ajout de colone
	cout << "ajout de colone" << endl;
	machine.addColumn(16);
	machine.addColumn(16);
	machine.addColumn(10);

	//cout << "données aléatoires" << endl;
	//on rentre les données
	machine.setWeightRandom();
	

	ifstream file("train-labels.idx1-ubyte",ios::binary);
	ifstream images_train("train-images.idx3-ubyte",ios::binary);
	

	//test1
	double accurency = 0;
	int good = 0, bad = 0;
	int numberCorrect = -1;
	unsigned char number;
	for(int i(0);i<100;i++)
	{
		file.seekg(8+i,ios::beg);
		file.read((char*)&number,1);
		numberCorrect =  int(number);
		

		images_train.seekg(16+i*784,ios::beg);
		memset(data,0,784);
		images_train.read(data,784);
		
		NetworkNeuron resultats(10,0);
		resultats.get_neuron(numberCorrect)->set_value(1);
		
		machine.setInput(data);
		machine.calcul();
		double max = 0;
		int indexMax = -1;
		for(int a(0);a<10;a++)
		{
			double v = machine.getOutput(a);
			//cout << " i:" << a << " ---> " << v << ((a!=numberCorrect)?"":"<<<<") << endl;
			if(v>max)
			{
				max=v;
				indexMax=a;
			}
		}
		if(indexMax==numberCorrect)
			good++;
		else
			bad++;
		accurency += machine.getPrecision(resultats);
	}
	accurency/=100.0;

	cout << "test1" << endl;
	cout << "accurency: " << accurency << " good:" << good << " bad:" << bad << endl;
	
	//train
	for(int i(0);i<60000;i++)
	{
		images_train.seekg(16+i*784,ios::beg);
		memset(data,0,784);
		images_train.read(data,784);
	
		int numberC = -1;
		unsigned char n;
		file.seekg(8+i,ios::beg);
		file.read((char*)&n,1);
		numberC =  int(n);

		NetworkNeuron resultats(10,0);
		resultats.get_neuron(numberC)->set_value(1);
		
		machine.setInput(data);
		machine.calcul();
		machine.train(resultats,LEARNING_RATE);
		//cout << "*";
	}
	cout << endl;

	
	//test2
	accurency = 0;
	good = 0;
	bad = 0;
	numberCorrect = -1;
	for(int i(0);i<100;i++)
	{
		file.seekg(8+i,ios::beg);
		file.read((char*)&number,1);
		numberCorrect =  int(number);
		

		images_train.seekg(16+i*784,ios::beg);
		memset(data,0,784);
		images_train.read(data,784);
		
		NetworkNeuron resultats(10,0);
		resultats.get_neuron(numberCorrect)->set_value(1);
		
		machine.setInput(data);
		machine.calcul();
		double max = 0;
		int indexMax = -1;
		for(int a(0);a<10;a++)
		{
			double v = machine.getOutput(a);
			//cout << " i:" << a << " ---> " << v << ((a!=numberCorrect)?"":"<<<<") << endl;
			if(v>max)
			{
				max=v;
				indexMax=a;
			}
		}
		if(indexMax==numberCorrect)
			good++;
		else
			bad++;
		accurency += machine.getPrecision(resultats);
	}
	accurency/=100.0;

	cout << "test2" << endl;
	cout << "accurency: " << accurency << " good:" << good << " bad:" << bad << endl;

	return 0;
}
