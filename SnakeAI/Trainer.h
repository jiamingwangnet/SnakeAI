#pragma once

#include <vector>
#include <DeepQLearning/NeuralNet.h>

class Trainer
{
public:
	Trainer(std::vector<dqn::NeuralNet::DataPoint>& data, size_t batchSize, float trainPercent);
public:
	void Train(dqn::NeuralNet& net, double learnRate, size_t index);

	const std::vector<std::vector<dqn::NeuralNet::DataPoint>>& GetTrainBatches() const;
	const std::vector<std::vector<dqn::NeuralNet::DataPoint>>& GetTestBatches() const;
private:
	std::vector<std::vector<dqn::NeuralNet::DataPoint>> trainBatches;
	std::vector<std::vector<dqn::NeuralNet::DataPoint>> testBatches;
};