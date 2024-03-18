#pragma once

#include "Matrix.h"
#include "NeuralNet.h"
#include <functional>
#include <vector>
#include <deque>
#include <fstream>
#include <iostream>

namespace net
{
	class Agent
	{
	public:
		using Action = std::function<void(void)>;
	protected:
		struct ReplayData
		{
			net::DMatrix state_t; // neural network inputs
			net::DMatrix action_t; // action index of the action list
			double reward_t;
			net::DMatrix state_next; // the resulting state
			bool done; // is terminal
		};

	public:
		Agent(const std::vector<Action>& actions)
			: actions{actions}
		{}

		virtual void SampleBegin(const net::DMatrix& state) = 0; // perform action
		virtual void SampleEnd(const net::DMatrix& newState, double reward, bool done) = 0; // observe reward

		virtual void Train() = 0;
		virtual void TrainBatch() = 0;
		virtual net::NeuralNet& GetMainNet() = 0;

		virtual void Save(std::string path, std::string name) = 0;
		virtual void Load(std::string path, std::string name) = 0;
	protected:
		virtual void TakeAction(const net::DMatrix& state) = 0;
		virtual void TakeAction(size_t index) = 0;

	protected:
		const std::vector<Action>& actions;

		net::DMatrix lastAction;
		net::DMatrix lastState;

		int steps = 0;
	};
}
