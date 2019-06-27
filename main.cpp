#include <iostream>
#include <fstream>

#include <cstdlib>
#include <ctime>

#include <string.h>
#include "m_learning.h"

using namespace std;
int main(int argc, char *argv[])
{
	cout << "initialisation" << endl;
	char data[784];

	cout << "lecture image dans le fichier" << endl;	
	//lecture de l'image dans un tableau en binaire
	ifstream images_train("train-images.idx3-ubyte",ios::binary);
	images_train.seekg(16,ios::beg);
	memset(data,0,784);
	images_train.read(data,784);
	
	cout << "lecture chiffre dans le fichier" << endl;
	//lecture du chiffre
	int numberCorrect = -1;
	ifstream file("train-labels.idx1-ubyte",ios::binary);
	unsigned char number;
	file.seekg(8,ios::beg);
	file.read((char*)&number,1);
	numberCorrect =  int(number);

	cout << "structure de données" << endl;
	//mise en place de la structure du Réseau de neurone
	MachineLearning machine(784);
	//ajout de colone
	cout << "ajout de colone" << endl;
	machine.addColumn(16);
	machine.addColumn(16);
	machine.addColumn(10);

	cout << "données image --->> dans la structure" << endl;
	//on rentre les données
	machine.setInput(data);
	machine.setWeightRandom();
	cout << "calcul(x)" << endl;
	machine.calcul();
	
	//test1
	NetworkNeuron resultats(10,0);
	resultats.get_neuron(numberCorrect)->set_value(1);
	cout << "résultat :" << endl;
	for(int i(0);i<machine.numberNeuronIn(machine.getNumberColumn()-1);i++)
	{
		cout << i << " --> " << machine.getOutput(i);
		if(i==numberCorrect)
			cout << "<<<<<<<<<<<<";
		cout << endl;
	}
	cout << "précision = " << machine.getPrecision(resultats) << endl;

	//train
	for(int i(0);i<10000;i++)
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
		machine.train(resultats);
	}

	//test2
	machine.setInput(data);
	machine.setWeightRandom();
	cout << "calcul(x)" << endl;
	machine.calcul();

	NetworkNeuron resultats2(10,0);
	resultats2.get_neuron(numberCorrect)->set_value(1);
	cout << "résultat :" << endl;
	for(int i(0);i<machine.numberNeuronIn(machine.getNumberColumn()-1);i++)
	{
		cout << i << " --> " << machine.getOutput(i);
		if(i==numberCorrect)
			cout << "<<<<<<<<<<<<";
		cout << endl;
	}
	cout << "précision = " << machine.getPrecision(resultats2) << endl;

	return 0;
}
