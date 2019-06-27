#ifndef LEARNING_DEFINE
#define LEARNING_DEFINE
 
#include <cmath>
#include <vector>

#include <cstdlib>

#include <ctime>
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
	void set_bias(double bias);
	double get_bias() const;
	int numberConnection() const;
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
	NetworkNeuron* getme();
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
	void setWeightRandom();
       	void calcul();	
	double getOutput(int index);
        int numberNeuronIn(int i);	
	void addColumn(int numberNeuron);
	int getNumberColumn() const; 
	double getPrecision(NetworkNeuron& result);
	void train(NetworkNeuron const& result);
protected:
	std::vector<NetworkNeuron> Lines;
};

#endif
