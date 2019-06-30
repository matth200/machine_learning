#include "m_learning.h"

using namespace std;

//fonction renvoyant dans un interval compris entre 0 et 1
double sigmoid(double a)
{
     return 1.0/(1.0+exp(-a));
}

//fonction qui renvoit la dérivé de la fonction sigmoid
double deriSigmoid(double a)
{
	return -pow(1-exp(-a),-2)*exp(-a);
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

void NetworkNeuron::backpropagation()
{

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
				Lines[l+1].get_neuron(j)->set_weight(i,rand()%1000/100.0-5.0);  
			}
			Lines[l+1].get_neuron(j)->set_bias(rand()%4000/100.0-20.0);
		}
	}
}

void MachineLearning::calcul()
{	
	double a = 0;
	for(int l(0);l<Lines.size()-1;l++)
	{
		//cout << "l:" << l+1 << "jMAX:" << Lines[l+1].get_number_neuron() << " iMAX:" << Lines[l].get_number_neuron() << "neuron connections [" << l+1 << "] = " << Lines[l+1].get_neuron(0)->numberConnection() << endl;
		for(int j(0);j<Lines[l+1].get_number_neuron();j++)
		{
			a = 0;
			for(int i(0);i<Lines[l].get_number_neuron();i++)
			{
				a+=Lines[l].get_neuron(i)->get_value()*Lines[l+1].get_neuron(j)->get_weight(i); 
			}
			Lines[l+1].get_neuron(j)->set_value(sigmoid(a+Lines[l+1].get_neuron(j)->get_bias()));	
			//cout << "neuroon l:" << l+1 << " j:" << j << " ---> " << Lines[l+1].get_neuron(j)->get_value() << endl;
		}
	}
}

double MachineLearning::getOutput(int index)
{	
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

void MachineLearning::train(NetworkNeuron& result)
{
	//retropropagation pour apprendre au réseaux on regarde les pentes de chaque variable pour se diriger vers une fonction de cout plus proche de 0
	bool first = 1;
	for(int l(Lines.size()-1);l>0;l--)
	{
		for(int j(0);j<Lines[l].get_number_neuron();j++)
		{
			double r = 0.3; //changement r
			double z = Lines[l].get_neuron(j)->get_bias();
			for(int x(0);x<Lines[l-1].get_number_neuron();x++)
			{
				z+=Lines[l-1].get_neuron(x)->get_value()*Lines[l].get_neuron(j)->get_weight(x);
			}
			double a = sigmoid(z);
			double y = result.get_neuron(j)->get_value();
			//on modifie le biais avec sa dérivée
 			Lines[l].get_neuron(j)->set_bias(Lines[l].get_neuron(j)->get_bias()-r*(deriSigmoid(z)*2.0*(a-y)));	
			//on modifie les poids avec leur dérivé aussi
			for(int i(0);i<Lines[l].get_neuron(j)->numberConnection();i++)
			{
				if(first)
				{
					Lines[l].get_neuron(j)->set_weight(i,Lines[l].get_neuron(j)->get_weight(i)-r*(Lines[l-1].get_neuron(i)->get_value()*deriSigmoid(z)*2.0*(a-y)));		
					Lines[l].get_neuron(j)->set_error(2.0*deriSigmoid(z)*(a-y));		
				}
				else{
					double deriError = 0;
					for(int k(0);k<Lines[l+1].get_number_neuron();k++)
					{
						deriError += Lines[l+1].get_neuron(k)->get_weight(j)*Lines[l+1].get_neuron(k)->get_error();
					}
					deriError *= 2.0*deriSigmoid(z);
					Lines[l].get_neuron(j)->set_error(deriError);
					Lines[l].get_neuron(j)->set_weight(i,Lines[l].get_neuron(j)->get_weight(i)-r*(Lines[l-1].get_neuron(i)->get_value()*deriSigmoid(z)*2.0*(deriError)));			
				}
			}
		}
		first = 0;
	}	
}

