#ifndef LEARNING_DEFINE
#define LEARNING_DEFINE
 
#include <cmath>
#include <vector>

#include <time.h>
#include <cstdlib>

#include <iostream>

double sigmoid(double a);
 
class Neuron
{
public:
	Neuron(int size, bool israndom);
	void set_value(double v);
	double get_value() const;
	void set_weight(int i, double weight);
	double get_weight(int i) const;
	double get_bias() const;
protected:
	double value;
	double b;
	std::vector<double> w;
};

class NetworkNeuron
{
public:
	NetworkNeuron(int size, NetworkNeuron *before);
	void set_after(NetworkNeuron *after);
	bool is_end() const;
	int get_number_neuron() const;
	Neuron* get_neuron(int index);
	void backpropagation();
protected:
	std::vector<Neuron> neurons;
	NetworkNeuron *beforeNetwork, *afterNetwork;
};

class MachineLearning
{
public:
	MachineLearning(int sizeInput);
	void setInput(char *data);
       	void calcul();	
	double getOutput(int index); 
	void addColumn(int numberNeuron);
	int getNumberColumn() const; 
	void train(NetworkNeuron const& result);
protected:
	std::vector<NetworkNeuron> Lines;
};

#endif
