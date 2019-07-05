#include "m_learning.h"

using namespace std;

//fonction renvoyant dans un interval compris entre 0 et 1
double sigmoid(double a)
{
     return 1.0/(1.0+exp(-a));
}

//fonction qui renvoit la dérivé de la fonction sigmoid
double sigmoidPrime(double a)
{
	return a * (1.0 - a);
}

Neuron::Neuron(int size, bool isRandom)
{
	//initialisation des valeurs
	value = 0;
	b = 0;
	deriError = 0;
	w.resize(size);
}

double Neuron::get_value() const
{
	return value;
}

void Neuron::set_value(double v)
{
	if(v>=0 && v<=1)
		value = v;
}

void Neuron::set_weight(int i, double weight)
{
	if(i>=0)
		w[i] = weight;
}

double Neuron::get_weight(int i) const
{ 
	return w[i]; 
}

void Neuron::set_bias(double bias)
{
	b = bias;
}

double Neuron::get_bias() const
{
	return b;
}

void Neuron::set_error(double error)
{
	deriError = error;
}

double Neuron::get_error() const
{
	return deriError;
}

int Neuron::numberConnection() const
{
 	return w.size();
}

NetworkNeuron::NetworkNeuron(int size, NetworkNeuron* before)
{
	neurons.resize(size,Neuron((before!=0)?before->get_number_neuron():0,1));
	beforeNetwork = before;
	afterNetwork = 0;
}

void NetworkNeuron::set_after(NetworkNeuron *after)
{
	afterNetwork = after;
}

bool NetworkNeuron::is_end() const
{
	return afterNetwork==0;
}

NetworkNeuron* NetworkNeuron::getme()
{
	return this;
}

int NetworkNeuron::get_number_neuron() const
{
	return neurons.size();
}

Neuron* NetworkNeuron::get_neuron(int index)
{
	return &(neurons[index]);	
}

MachineLearning::MachineLearning(int sizeInput)
{
	Lines.push_back(NetworkNeuron(sizeInput,0));
}

void MachineLearning::setInput(char *data)
{
	double value = 0;
	for(int i(0);i<784;i++)
	{
		value = (*((unsigned char*)(data+i)))/255.0;
		Lines[0].get_neuron(i)->set_value(value);
	}
}

void MachineLearning::setWeightRandom()
{
	srand(time(NULL));
	for(int l(0);l<Lines.size()-1;l++)
	{
		for(int j(0);j<Lines[l+1].get_number_neuron();j++)
		{
			for(int i(0);i<Lines[l+1].get_neuron(j)->numberConnection();i++)
			{
				Lines[l+1].get_neuron(j)->set_weight(i,rand()%2000/1000.0-1.0);  
			}
			Lines[l+1].get_neuron(j)->set_bias(rand()%2000/1000.0-1.0);
		}
	}
}

void MachineLearning::calcul()
{	
	double a = 0;
	for(int l(0);l<Lines.size()-1;l++)
	{
		for(int j(0);j<Lines[l+1].get_number_neuron();j++)
		{
			a = 0;
			for(int i(0);i<Lines[l].get_number_neuron();i++)
			{
				a+=Lines[l].get_neuron(i)->get_value()*Lines[l+1].get_neuron(j)->get_weight(i); 
			}
			Lines[l+1].get_neuron(j)->set_value(sigmoid(a+Lines[l+1].get_neuron(j)->get_bias()));	
		}
	}
}

double MachineLearning::getZ(int l, int j)
{
	double a = 0;
	for(int i(0);i<Lines[l-1].get_number_neuron();i++)
	{
		a+=Lines[l-1].get_neuron(i)->get_value()*Lines[l].get_neuron(j)->get_weight(i); 
	}
	return a+Lines[l].get_neuron(j)->get_bias();
}

double MachineLearning::getOutput(int index)
{
	//if(index==0)
		//cout << "--------------------" << endl;	
	//cout << "Error: " << Lines[Lines.size()-1].get_neuron(index)->get_error() << endl;
	return Lines[Lines.size()-1].get_neuron(index)->get_value();
}

void MachineLearning::addColumn(int numberNeuron)
{
	//Lines[Lines.size()-1].get_number_neuron();
	Lines.push_back(NetworkNeuron(numberNeuron,Lines[Lines.size()-1].getme()));
	Lines[Lines.size()-2].set_after(Lines[Lines.size()-1].getme());
}

int MachineLearning::numberNeuronIn(int i)
{
	return Lines[i].get_number_neuron();
}

int MachineLearning::getNumberColumn() const
{
	return Lines.size();
}

double MachineLearning::getPrecision(NetworkNeuron& result)
{
	double diff = 0;
	for(int i(0);i<Lines[Lines.size()-1].get_number_neuron();i++)
	{
		diff+=pow(getOutput(i)-result.get_neuron(i)->get_value(),2);
	}
	return diff;
}

void MachineLearning::train(NetworkNeuron& result, double r)
{
	//retropropagation des erreurs
	for(int l(Lines.size()-1);l>0;l--)
	{
		for(int j(0);j<Lines[l].get_number_neuron();j++)
		{
			double error = 0.0;
			//si c'est dans les couches cachés
			if(l!=Lines.size()-1)
			{
				for(int i(0);i<Lines[l+1].get_number_neuron();i++)
				{	
					Neuron &neuron = *Lines[l+1].get_neuron(i);
				        error+=neuron.get_weight(j)*neuron.get_error();
				}
			}
			//si c'est la couche de sortie des neurones
			else{
				error = getOutput(j)-result.get_neuron(j)->get_value();
			}
			Neuron &neuron = *Lines[l].get_neuron(j);
			neuron.set_error(error * sigmoidPrime(neuron.get_value()));			
		}
	}
	
	//mise à jour des biais et des poids
	for(int l(Lines.size()-1);l>0;l--)
	{
		for(int j(0);j<Lines[l].get_number_neuron();j++)
		{
			Neuron &neuron = *Lines[l].get_neuron(j);
			//bias
			neuron.set_bias(neuron.get_bias()-r*neuron.get_error());
			//weight
			for(int i(0);i<Lines[l].get_neuron(j)->numberConnection();i++)
			{
				neuron.set_weight(i,neuron.get_weight(i)-r*neuron.get_error()*Lines[l-1].get_neuron(i)->get_value());
			}
		}
	}
}

void MachineLearning::saveTraining(const char *file)
{
	ofstream f(file,ios::binary);
	f.seekp(0,ios::beg);
	
	//on enlève 1 parce que la couche en entrée n'a pas de poid ou de biais
	int nbrNeuron = getNumberColumn();		
	f.write((char*)&nbrNeuron,sizeof(nbrNeuron));
	
	int cursor = sizeof(nbrNeuron);
	for(int i(0);i<nbrNeuron;i++)
	{
			
	}
}

void MachineLearning::backupTraining(const char *file)
{
	ifstream f(file,ios::binary);

} 
