#include "Trainer.h"
#include <cmath>
#include <DeepQLearning/CostFuncs.h>

Trainer::Trainer(std::vector<dqn::NeuralNet::DataPoint>& data, size_t batchSize, float trainPercent)
{
	size_t trainSize = (size_t)std::floorf((float)data.size() * trainPercent);
	size_t testSize = data.size() - trainSize;

	std::vector<dqn::NeuralNet::DataPoint> batch;

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

void Trainer::Train(dqn::NeuralNet& net, double learnRate, size_t index)
{
	dqn::MSE<double> mse;
	net.GradientDescent(learnRate, trainBatches[index], mse);
}

const std::vector<std::vector<dqn::NeuralNet::DataPoint>>& Trainer::GetTrainBatches() const
{
	return trainBatches;
}

const std::vector<std::vector<dqn::NeuralNet::DataPoint>>& Trainer::GetTestBatches() const
{
	return testBatches;
}
