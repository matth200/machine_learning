#ifndef LEARNING_DEFINE
#define LEARNING_DEFINE
 
#include <cmath>
#include <vector>
#include <time.h>


double sigmoid(double a);

class Neuron
{
public:
	Neuron(bool israndom);
protected:
	double value;
	std::vector<double> w;
};

class NetworkNeuron
{
public:
	NetworkNeuron();
	void getValue() const;
	void backPropagation();
protected:
	std::vector<Neuron> neurons;
	NetworkNeuron *beforeNetwork, *afterNetwork;
	double b;
};

class MachineLearning
{
public:
	MachineLearning(int sizeInput);
	bool setInput(int index, double value);
	bool getOutput(int index);
	void addColumn(int numberNeuron);
	void getNumberColumn();
	bool getReady() const;
	void train(const char* filename, NetworkNeuron& const result);
protected:
	std::vector<NetworkNeuron> Lines;
	std::vector<double> inputs;
};

#endif
