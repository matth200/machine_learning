#ifndef LEARNING_DEFINE
#define LEARNING_DEFINE
 
#include <cmath>
#include <vector>

#include <time.h>
#include <cstdlib>

double sigmoid(double a);
 
class Neuron
{
public:
	Neuron(int size, bool israndom);
	void set_value(double v);
	double get_value() const;
	void set_weight(int i, double weight);
	double get_weight(int i);

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
	Neuron* get_neuron(int index);
	void backPropagation();
protected:
	std::vector<Neuron> neurons;
	NetworkNeuron *beforeNetwork, *afterNetwork;
};

class MachineLearning
{
public:
	MachineLearning(int sizeInput);
	bool setInput(int index, double value);
	double getOutput(int index);
	void addColumn(int numberNeuron);
	int getNumberColumn() const;
	void train(const char* filename, NetworkNeuron& const result);
protected:
	std::vector<NetworkNeuron> Lines;
	std::vector<double> inputs;
};

#endif
