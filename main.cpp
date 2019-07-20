#include <iostream>
#include <fstream>

#include <cstdlib>
#include <ctime>

#include <string.h>
#include "m_learning.h"

using namespace std;

void testMachine(MachineLearning &machine, ifstream &fileNumbers, ifstream &fileImages, int numberTest);

int main(int argc, char *argv[])
{
	srand(time(NULL));
	cout << "initialisation" << endl;
	char data[784];
	
	if(argc<2)
	{
		cout << argv[0] << " fichier_sauvegarde (fichier_recuperation)" << endl;
		exit(0);
	}
	
	MachineLearning machine(784);
	//ajout de colone
	cout << "ajout de colone" << endl;
	
	//hidden layers
	machine.addColumn(40);
	machine.addColumn(20);
	machine.addColumn(20);	
	//output
	machine.addColumn(10);

	
	//on rentre les données
	if(argc==2)
	{
		cout << "poids et biais aléatoires" << endl;
		machine.setWeightRandom();
	}
	else if(argc==3)
	{
		cout << "Récupération de la sauvegarde dans \"" << argv[2] << "\"" << endl; 
		machine.backupTraining(argv[2]);
	}

	//fichier avec les données
	ifstream file("train-labels.idx1-ubyte",ios::binary);
	ifstream images_train("train-images.idx3-ubyte",ios::binary);
	
	//test1
	cout << "Test 1" << endl;
	testMachine(machine,file,images_train,1000);

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
		machine.train(resultats,0.00025);
	}
	
	cout << endl;
	
	//test2
	cout << "Test 2" << endl;
	testMachine(machine,file,images_train,1000);	
	
	machine.saveTraining(argv[1]);
	return 0;
}

void testMachine(MachineLearning &machine, ifstream &fileNumbers, ifstream &fileImages, int numberTest)
{
	char data[789];
	double accurency = 0;
	int good = 0, bad = 0;
	int numberCorrect = -1;
	unsigned char number;
	for(int i(0);i<numberTest;i++)
	{
		fileNumbers.seekg(8+i,ios::beg);
		fileNumbers.read((char*)&number,1);
		numberCorrect =  int(number);
		

		fileImages.seekg(16+i*784,ios::beg);
		memset(data,0,784);
		fileImages.read(data,784);
		
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
	accurency/=double(numberTest);

	cout << "accurency: " << accurency << " good:" << good/double(numberTest)*100.0 << " bad:" << bad/double(numberTest)*100.0 << endl;

	int numberRandom = rand()%1000;
	fileNumbers.seekg(8+numberRandom,ios::beg);
	fileNumbers.read((char*)&number,1);
	numberCorrect =  int(number);

	fileImages.seekg(16+numberRandom*784,ios::beg);
	memset(data,0,784);
	fileImages.read(data,784);
		
	machine.setInput(data);
	machine.calcul();
	cout << "--> prédiction sur le chiffre " << numberCorrect << endl;
	for(int a(0);a<10;a++)
	{
		cout << a << ". " << machine.getOutput(a);
	       	if(a==numberCorrect)
			cout << " <<<<";
		cout << endl;	
	}
}

