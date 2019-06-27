#include "m_learning.h"

using namespace std;

srand(time(NULL));

//fonction renvoyant dans un interval compris entre 0 et 1
double sigmoid(double a)
{
     return 1.0/(1.0+exp(-x));
}

Neuron::Neuron(int size, bool isRandom)
{
	//initialisation des valeurs
	value = 0;
	b = 0;
	w.resize(size,0);

	//si aléatoire
	if(isRandom)
	{
		for(int i(0);w.size();i++)
		{
			//entre -5 et 5
			w[i] = rand()%1000/100.0-5.0;
		}
		b = rand%4000/100.0-20.0;
	}	
}

double Neuron::get_value()
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

double Neuron::get_weight(int i)
{
	return w[i]; 
}

double Neuron::get_bias()
{
	return b;
}

NetworkNeuron::NetworkNeuron(int size, NetworkNeuron* before)
{
	neurons.resize(size,Neuron((before!=0)?before->get_number_neuron():0,1);
	beforeNetwork = before;
	afterNetwork = 0;
}

void NetworkNeuron::set_after(NetworkNeuron *after)
{
	afterNetwork = after;
}

bool NetworkNeuron::is_end()
{
	return afterNetwork==0;
}

Neuron* NetworkNeuron::get_neuron(int index)
{
	return &neuron[index];	
}

void NetworkNeuron::backpropagation()
{

}

MachineLearning::MachineLearning(int sizeInput)
{
	Lines.push_back(NetworkNeuron(sizeInput,0));
}

void MachineLearning::setInput(char *data)
{
	if(index>=0 && value>=0 && value<=1)
	{
		double value = 0;
		for(int i(0);i<786;i++)
		{
			value = (*((unsigned char*)(data+i)))/255.0;
			Lines[0].get_neuron(i)->set_value(value);
		}
	}
}

void MachineLearning::calcul()
{
	double a;
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

double MachineLearning::getOutput(int index)
{	
	return Lines[Lines.size()-1].get_neuron(index)->get_value();
}

void MachineLearning::addColumn(int numberNeuron)
{
	Lines.push_back(NetworkNeuron(numberNeuron,&(Lines[Lines.size()-1])));
	Lines[Lines.size()-2].set_after(&(Lines[Lines.size()-1]));
}

int MachineLearning::getNumberColumn()
{
	return Lines.size();
}

void MachineLearning::train(unsigned char* example, NetworkNeuron& const result)
{
	
}

