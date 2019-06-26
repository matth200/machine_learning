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
	value = 0;
	w.resize(size);
	for(int i(0);w.size();i++)
	{
		//entre -5 et 5
		w[i] = rand()%1000/100.0-5.0;
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


NetworkNeuron::NetworkNeuron(int size, NetworkNeuron* before)
{
	neurons.resize(size);
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

bool MachineLearning::setInput(int index, double value)
{
	if(index>=0 && value>=0 && value<=1)
		Lines[0].get_neuron(index)->set_value(value);
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
