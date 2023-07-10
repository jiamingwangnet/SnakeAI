#include "Layer.h"
#include "Utility.h"

net::Layer::Layer(LayerInfo info)
	: biases{1, info.nodes}, n_nodes{ info.nodes }, weights{ info.in_nodes, n_nodes }, activation{ info.activation }
{
	for (double& w : weights)
	{
		w = Util::Random<double>(std::uniform_real_distribution<double>(WEIGHT_MIN, WEIGHT_MAX)) / std::sqrt((double)info.in_nodes);
	}
}

net::DMatrix net::Layer::Forward(const DMatrix& input, bool start)
{
	if (start)
	{
		outputs = input;
		return input;
	}

	weightedInputs = input * weights + biases;
	outputs = activation->Activate(weightedInputs);
	return outputs;
}

const net::DMatrix& net::Layer::GetWeights() const
{
	return weights;
}

void net::Layer::SetWeights(const DMatrix& value)
{
	weights = value;
}

const net::DMatrix& net::Layer::GetBiases() const
{
	return biases;
}

void net::Layer::SetBiases(const DMatrix& value)
{
	biases = value;
}

const net::DMatrix& net::Layer::GetWeightedInputs() const
{
	return weightedInputs;
}

const net::DMatrix& net::Layer::GetOutputs() const
{
	return outputs;
}