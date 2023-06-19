#include "Layer.h"
#include "Utility.h"

dqn::Layer::Layer(LayerInfo info)
	: biases{1, info.nodes}, n_nodes{ info.nodes }, weights{ info.in_nodes, n_nodes }, activation{ info.activation }
{
	for (double& w : weights)
	{
		w = Util::Random<double>(std::uniform_real_distribution<double>(WEIGHT_MIN, WEIGHT_MAX)) / std::sqrt((double)info.in_nodes);
	}
}

dqn::DMatrix dqn::Layer::Forward(const DMatrix& input, bool start)
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

const dqn::DMatrix& dqn::Layer::GetWeights() const
{
	return weights;
}

void dqn::Layer::SetWeights(const DMatrix& value)
{
	weights = value;
}

const dqn::DMatrix& dqn::Layer::GetBiases() const
{
	return biases;
}

void dqn::Layer::SetBiases(const DMatrix& value)
{
	biases = value;
}

const dqn::DMatrix& dqn::Layer::GetWeightedInputs() const
{
	return weightedInputs;
}

const dqn::DMatrix& dqn::Layer::GetOutputs() const
{
	return outputs;
}