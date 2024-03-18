#pragma once

#include "Agent.h"
#include "Layer.h"
#include <vector>
#include <deque>
#include <stdexcept>

namespace ppo
{
	class PPOAgent : public net::Agent
	{
	private:
		struct PPOData : public ReplayData
		{
			size_t t;
			net::DMatrix policyold;
		};
	public:
		PPOAgent(const std::vector<Action>& actions, std::vector<net::LayerInfo> layerInfo, size_t replaySize);

		void SampleBegin(const net::DMatrix& state);
		void SampleEnd(const net::DMatrix& newState, double reward, bool done);

		void Train();
		void TrainBatch();

		net::NeuralNet& GetMainNet() override { return actor; };

		void Save(std::string path, std::string name) override {}
		void Load(std::string path, std::string name) override {}
	protected:
		void TakeAction(const net::DMatrix& state);
		void TakeAction(size_t index) override { throw std::logic_error{ "NOT IMPLEMENTED." }; }
	private:
		// calculate advantage using Generalized Advantage Estimation (GAE) for every step t
		std::vector<double> CalculateAdvantage(const std::vector<PPOData>& batch);

	private:
		static constexpr size_t BATCH_SIZE = 10; // total steps to run before optimization
		std::deque<PPOData> memory;

		net::NeuralNet actor; // policy
		net::NeuralNet critic;

		size_t time = 0;
		
		net::DMatrix oldOutput;
		double discount = 0.99;
		double gaeParam = 0.95; // lambda

		size_t replaySize;
	};
}

