#include "PPOAgent.h"
#include "ActivationFuncs.h"
#include "Utility.h"
#include "CostFuncs.h"

ppo::PPOAgent::PPOAgent(const std::vector<Action>& actions, std::vector<net::LayerInfo> layerInfo, size_t replaySize)
	: Agent{actions}, replaySize{ replaySize }
{
	/*
		Seperating actor and critic function
		because i couldnt get the shared parameters working
	*/

	// TODO: attempt to implement shared parameters

	layerInfo.back().activation = std::make_shared<net::Softmax>(); // last activation must be softmax
	actor = net::NeuralNet{ layerInfo };

	layerInfo.back().activation = std::make_shared<net::Linear>();
	critic = net::NeuralNet{ layerInfo };
}

void ppo::PPOAgent::SampleBegin(const net::DMatrix & state)
{
	using namespace net;

	lastState = state;

	TakeAction(state);
}

void ppo::PPOAgent::SampleEnd(const net::DMatrix & newState, double reward, bool done)
{
	using namespace net;

	memory.emplace_back();

	PPOData& data = memory.back();

	data.t = time;
	data.action_t = lastAction;
	data.done = done;
	data.reward_t = reward;
	data.state_next = newState;
	data.state_t = lastState;
	data.policyold = oldOutput;

	if (memory.size() > replaySize)
		memory.pop_front();

	time++;
}

void ppo::PPOAgent::Train()
{

	// https://en.wikipedia.org/wiki/Entropy_(information_theory)#Example
	/*double entropy = 0.0;
	for (double& val : policynew)
	{
		entropy += val * Util::LogBase((double)policynew.GetSize(), val);
	}*/
	//double entropyDeriv = 0.0;
	//for (double& val : policynew) // derivative of entropy
	//{
	//	entropy += val/( val * log( (double)policynew.GetSize() ) ) + Util::LogBase((double)policynew.GetSize(), val);
	//}

	using namespace net;

	if (time % 200 != 0 || time == 0) return;

	std::vector<PPOData> batch;
	Util::Sample<PPOData, std::deque<PPOData>>(memory.begin(), memory.end(), BATCH_SIZE, batch);

	// optimization code

	std::vector<double> adv = CalculateAdvantage(batch);

	// Pseudocode in DeepMimic paper arXiv:1804.02717v3 [cs:GR] 27 Jul 2018
	// https://arxiv.org/pdf/1804.02717.pdf

	// update value function
	for (PPOData& data : batch)
	{
		// calculate critic values using Deep Q Learning
		DMatrix target = critic.CalculateOutputs(data.state_t);
		DMatrix newVal = critic.CalculateOutputs(data.state_next);

		size_t index = (size_t)(std::max_element(data.action_t.begin(), data.action_t.end()) - data.action_t.begin());
		target(0, index)
			= data.reward_t + discount * *std::max_element(newVal.begin(), newVal.end()) * (1.0 - (double)data.done);

		DMatrix output = critic.CalculateOutputs(data.state_t);
		
		MSE<double> cost;
		DMatrix grad = cost.Derivative(output, target);

		critic.GradientDescent(0.001, grad); // TODO: change to use average gradients. Make sure to use the right output and weighted inputs for differentiation
	}

	// update policy
	size_t i = 0;
	for (PPOData& data : batch)
	{
		//std::cout << adv[i] << '\n';
		// calculate actor values using PPO objective
		DMatrix output = actor.CalculateOutputs(data.state_t);

		CLIP<double> objective{adv[i], 0.2};
		DMatrix grad = objective.Derivative(output, data.policyold);

		actor.GradientDescent(0.001, grad); // TODO: change to use average gradients

		i++;
	}

	batch.clear();
}

void ppo::PPOAgent::TrainBatch()
{}

void ppo::PPOAgent::TakeAction(const net::DMatrix & state)
{
	using namespace net;

	lastAction = DMatrix{ 1, actions.size() };

	DMatrix output = actor.CalculateOutputs(state);
	std::vector<double> vec = (std::vector<double>)output;

	size_t index = Util::RandomFromDistribution(vec);

	lastAction(0, index) = 1.0;
	oldOutput = output;

	actions[index]();
}

std::vector<double> ppo::PPOAgent::CalculateAdvantage(const std::vector<PPOData>& batch) // TODO: fix pls idk how this works
{
	using namespace net;	

	static constexpr int TIME_RANGE = 5;

	std::vector<double> returns;
	for (const PPOData& data : batch) // TODO: calculate advantage for each element in the batch by getting the states from memory deque
	{
		double lastAdv = 0.0;
		//https://nn.labml.ai/rl/ppo/gae.html
		
		for (size_t i = data.t > TIME_RANGE ? (data.t - TIME_RANGE) : 0; i <= data.t; i++) // limit starting value
		{
			double mask = 1.0 - (double)memory[i].done;

			DMatrix valuen = critic.CalculateOutputs(data.state_next);
			DMatrix value = critic.CalculateOutputs(data.state_t);

			double delta = data.reward_t + discount * *std::max_element(valuen.begin(), valuen.end()) * mask - *std::max_element(value.begin(), value.end());
			lastAdv = delta + discount * gaeParam * lastAdv;
		}

		returns.push_back(lastAdv);
	}

	return returns;
}
