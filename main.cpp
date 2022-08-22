#include <iostream>
#include <fstream>

#include <cstdlib>
#include <ctime>

#include <chrono>
#include <thread>

#include <cmath>
#include <cstring>

#include <string.h>
#include "src/m_learning.h"

#define UPDATE_STATS_EVERY 1000
#define LEARNING_RATE 0.4
#define NUMBER_DATA 60000.0

using namespace std;

typedef chrono::high_resolution_clock::time_point time_point;


string loading_text(int i, int size){
	string texte = "[";
	int index = int(i/100.0*size);
	for(int i(0);i<size;i++){
		if(i<index){
			texte += '#';
		}else{
			texte += ' ';
		}
	}
	texte+=']';
	return texte;
}

int main(int argc, char *argv[])
{
	srand(time(0));

	cout << "Initialisation" << endl;
	char data[784];
	
	MachineLearning machine(784);
	//ajout de colone
	cout << "Ajout des colonnes aux réseaux de neuronnes" << endl;
	machine.addColumn(16);
	machine.addColumn(16);
	machine.addColumn(10);

	cout << "Poids et biais définit par des valeurs aléatoires" << endl;
	//on rentre les données
	machine.setWeightRandom();
	

	cout << "Chargement de la base de données" << endl;
	ifstream file("database/train-labels.idx1-ubyte",ios::binary);
	ifstream images_train("database/train-images.idx3-ubyte",ios::binary);
	

	//test1
	cout << "Test 1" << endl;
	time_point start_time = chrono::high_resolution_clock::now();

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
		double duration = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now()-start_time).count()/1000.0;
		cout << "\r" << duration << "s, accuracy:" << accurency/(i+1) << " good:" << good << " bad:" << bad << " total:" << i+1 << flush; 
	}

	cout << endl;
	
	//train
	start_time = chrono::high_resolution_clock::now();
	cout << "Entrainement..." << endl;
	accurency = 0.0;
	good = 0;
	bad = 0;
	for(int i(0);i<NUMBER_DATA;i++)
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
		if(indexMax==numberC)
			good++;
		else
			bad++;
		accurency += machine.getPrecision(resultats);


		machine.train(resultats,LEARNING_RATE);
		
		if(int(i/UPDATE_STATS_EVERY)!=int((i+1)/UPDATE_STATS_EVERY)){
			//affichage stats
			double duration = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now()-start_time).count()/1000.0;
			int loading_level = int(i/NUMBER_DATA*100.0)+1;
			cout << "\r" << int(duration) << "s, accuracy:" << accurency/UPDATE_STATS_EVERY << " accurate:" << double(good)/(good+bad)*100.0 << "%,  loaded " << loading_text(loading_level, 30) << " " << loading_level << "%        " << flush; 

			//on initialise
			accurency = 0.0;
			good = 0;
			bad = 0;
		}
	}
	cout << endl;

	
	//test2
	start_time = chrono::high_resolution_clock::now();
	cout << "Test 2" << endl;
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
		double duration = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now()-start_time).count()/1000.0;
		cout << "\r" << duration << "s, accuracy:" << accurency/(i+1) << " good:" << good << " bad:" << bad << " total:" << i+1 << flush; 
	}
	cout << endl;

	return 0;
}
