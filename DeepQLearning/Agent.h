#pragma once

#include "Matrix.h"
#include "NeuralNet.h"
#include <functional>
#include <vector>
#include <deque>
#include <fstream>
#include <iostream>

namespace dqn
{
	class Agent
	{
	public:
		using Action = std::function<void(void)>;
	private:
		struct ReplayData
		{
			DMatrix state_t; // neural network inputs
			DMatrix action_t; // action index of the action list
			double reward_t;
			DMatrix state_next; // the resulting state
			bool done; // is terminal
		};

	public:
		Agent(const std::vector<Action>& actions, const std::vector<LayerInfo>& layerinfo, int replaySize, double epsilonDecay);

		void Load(std::string path, std::string name)// FIXME
		{
			actionValueFunc.Load(path + name + ".actvalue");
			targetActionValueFunc.Load(path + name + ".targvalue");

			std::ifstream agentFile{ name + ".agtvalue", std::ios::binary };
			if (!agentFile) std::cout << name + ".agtvalue could not be opened.";

			agentFile.read(reinterpret_cast<char*>(&epsilon), sizeof(epsilon));
			agentFile.read(reinterpret_cast<char*>(&steps), sizeof(steps));

			/*size_t replaySize;
			agentFile.read(reinterpret_cast<char*>(&replaySize), sizeof(replaySize));
			replays.resize(replaySize);

			for (ReplayData& replay : replays)
			{
				agentFile.read(reinterpret_cast<char*>(&replay.action_t), sizeof(replay.action_t));
				agentFile.read(reinterpret_cast<char*>(&replay.done), sizeof(replay.done));
				agentFile.read(reinterpret_cast<char*>(&replay.reward_t), sizeof(replay.reward_t));
				agentFile.read(reinterpret_cast<char*>(&replay.state_next), sizeof(replay.state_next));
				agentFile.read(reinterpret_cast<char*>(&replay.state_t), sizeof(replay.state_t));
			}*/

			agentFile.close();
		}
		void Save(std::string path, std::string name)// FIXME
		{
			actionValueFunc.Save(path + name + ".actvalue");
			targetActionValueFunc.Save(path + name + ".targvalue");
			std::ofstream agentFile{ name + ".agtvalue", std::ios::binary };

			agentFile.write(reinterpret_cast<const char*>(&epsilon), sizeof(epsilon));
			agentFile.write(reinterpret_cast<const char*>(&steps), sizeof(steps));

			/*size_t replaySize = replays.size();
			agentFile.write(reinterpret_cast<const char*>(&replaySize), sizeof(replaySize));

			for (const ReplayData& replay : replays)
			{
				agentFile.write(reinterpret_cast<const char*>(&replay.action_t  ), sizeof(replay.action_t  ));
				agentFile.write(reinterpret_cast<const char*>(&replay.done	    ), sizeof(replay.done	   ));
				agentFile.write(reinterpret_cast<const char*>(&replay.reward_t  ), sizeof(replay.reward_t  ));
				agentFile.write(reinterpret_cast<const char*>(&replay.state_next), sizeof(replay.state_next));
				agentFile.write(reinterpret_cast<const char*>(&replay.state_t   ), sizeof(replay.state_t   ));
			}*/

			agentFile.close();
		}

		void SampleBegin(const DMatrix& state); // perform action
		void SampleEnd(const DMatrix& newState, double reward, bool done); // observe reward

		void Train();
		void TrainBatch();

	private:
		void TakeAction(const DMatrix& state);
		void TakeAction(size_t index);

	private:
		static constexpr int RESET_RATE = 500;
		static constexpr int DECAY_DELAY = 1;
		static constexpr int LEARN_EVERY = 1;
		static constexpr int BATCH_SIZE = 100;

		const double epsilonDecay; // 0.999994
		double epsilon = 1.0;
		double discount = 0.9;

		std::deque<ReplayData> replays;
		int replaySize;

		const std::vector<Action>& actions;

		NeuralNet actionValueFunc;
		NeuralNet targetActionValueFunc;

		DMatrix lastAction;
		DMatrix lastState;

		int steps = 0;
	};
}
