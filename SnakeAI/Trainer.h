#pragma once

#include <vector>
#include <ReinforcementLearning/NeuralNet.h>

class Trainer
{
public:
	Trainer(std::vector<net::NeuralNet::DataPoint>& data, size_t batchSize, float trainPercent);
public:
	void Train(net::NeuralNet& net, double learnRate, size_t index);

	const std::vector<std::vector<net::NeuralNet::DataPoint>>& GetTrainBatches() const;
	const std::vector<std::vector<net::NeuralNet::DataPoint>>& GetTestBatches() const;
private:
	std::vector<std::vector<net::NeuralNet::DataPoint>> trainBatches;
	std::vector<std::vector<net::NeuralNet::DataPoint>> testBatches;
};