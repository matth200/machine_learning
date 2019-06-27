#include <iostream>
#include <fstream>

#include <string.h>
#include "m_learning.h"

using namespace std;
int main(int argc, char *argv[])
{
	cout << "lecture" << endl;
	char data[786];
	
	//lecture de l'image dans un tableau en binaire
	ifstream images-train("train-images.idx3-ubyte",ios::binary);
	images-train.seekg(16,ios:beg);
	memset(&data,0,786);
	images-train.read(&data,786);
	
	//lecture du chiffre
	int numberCorrect = -1;
	ifstream file("train-labels.idx1-ubyte",ios::binary);
	unsigned char number;
	file.seekg(8,ios::beg);
	file.read((char*)&number,1);
	numberCorrect =  int(number);

	//mise en place de la structure du Réseau de neurone
	MachineLearning machine(784);
	machine.addColumn(16);
	machine.addColumn(16);
	machine.addColumn(10);

	//on rentre les données
	machine.setInput(data);
	machine.calcul();
	
	cout << "résultat :" << endl;
	for(int i(0);i<10;i++)
	{
		cout << i << " --> " << machine.getOutput(i);
		if(i==numberCorrect)
			cout << "<<<<<<<<<<<<";
		cout << endl;
	}
	return 0;
}
