#include "Trainer.h"
#include <cmath>
#include <ReinforcementLearning/CostFuncs.h>
#include <stdexcept>

Trainer::Trainer(std::vector<net::NeuralNet::DataPoint>& data, size_t batchSize, float trainPercent)
{
	size_t trainSize = (size_t)std::floorf((float)data.size() * trainPercent);
	size_t testSize = data.size() - trainSize;

	std::vector<net::NeuralNet::DataPoint> batch;

	for (size_t i = 0; i < trainSize; i++)
	{
		batch.push_back(data[i]);
		if (i != 0 && i % batchSize == 0)
		{
			trainBatches.push_back(batch);
			batch.clear();
		}
	}
	if (batch.size() != 0)
	{
		trainBatches.push_back(batch);
		batch.clear();
	}

	for (size_t i = 0; i < testSize; i++)
	{
		batch.push_back(data[i]);
		if (i != 0 && i % batchSize == 0)
		{
			testBatches.push_back(batch);
			batch.clear();
		}
	}
	if (batch.size() != 0)
	{
		testBatches.push_back(batch);
		batch.clear();
	}
}

void Trainer::Train(net::NeuralNet& net, double learnRate, size_t index)
{
	throw std::logic_error{ "not implemented" };
	/*net::MSE<double> mse;
	net.GradientDescent(learnRate, trainBatches[index], mse);*/
}

const std::vector<std::vector<net::NeuralNet::DataPoint>>& Trainer::GetTrainBatches() const
{
	return trainBatches;
}

const std::vector<std::vector<net::NeuralNet::DataPoint>>& Trainer::GetTestBatches() const
{
	return testBatches;
}
