#include "Agent.h"
#include "Utility.h"
#include "CostFuncs.h"

#ifdef DEBUG_LOG
#include <fstream>

#define FILEOUT "debug.txt"
#endif

dqn::Agent::Agent(const std::vector<Action>& actions, const std::vector<LayerInfo>& layerinfo, int replaySize, double epsilonDecay)
	: actions{ actions }, actionValueFunc{ layerinfo }, targetActionValueFunc{ actionValueFunc }, epsilonDecay{ epsilonDecay }, replaySize{ replaySize }
{
}

void dqn::Agent::SampleBegin(const DMatrix& state)
{
	lastState = state;

	if (Util::RandomChance(epsilon))
	{
		TakeAction(Util::Random<size_t>(std::uniform_int_distribution<size_t>(0, actions.size() - 1)));
	}
	else
	{
		TakeAction(state);
	}
}

void dqn::Agent::SampleEnd(const DMatrix& newState, double reward, bool done)
{
	replays.emplace_back();
	ReplayData& replay = replays.back();
	replay.action_t = lastAction;
	replay.state_t = lastState;
	replay.reward_t = reward;
	replay.done = done;

	replay.state_next = newState;

	if (replays.size() > replaySize)
		replays.pop_front();

	if(steps % DECAY_DELAY == 0) epsilon *= epsilonDecay;
}

void dqn::Agent::Train()
{
	steps++;

	if (steps % LEARN_EVERY != 0) return;

	const ReplayData& replay = replays[Util::Random<size_t>(std::uniform_int_distribution<size_t>(0, replays.size() - 1))];

	NeuralNet::DataPoint data;
	data.predicted = actionValueFunc.CalculateOutputs(replay.state_t);

	DMatrix target = actionValueFunc.CalculateOutputs(replay.state_t);
	DMatrix newVal = targetActionValueFunc.CalculateOutputs(replay.state_next);

	size_t index = (size_t)(std::max_element(replay.action_t.begin(), replay.action_t.end()) - replay.action_t.begin());

	target(0, index)
		= replay.reward_t + discount * *std::max_element(newVal.begin(), newVal.end()) * (1.0 - (double)replay.done);

	MSE<double> mse;
	DMatrix output = actionValueFunc.CalculateOutputs(replay.state_t);
	
	data.input = replay.state_t;
	data.expected = target;
	
	actionValueFunc.GradientDescent(0.001, data, mse);

	if (steps % RESET_RATE == 0)
	{
		targetActionValueFunc = actionValueFunc;
	}
}


void dqn::Agent::TrainBatch()
{
	throw std::exception{ "Not Implemented" };
}

void dqn::Agent::TakeAction(const DMatrix& state)
{
	lastAction = DMatrix{ 1, actions.size() };

	DMatrix actionMat = actionValueFunc.CalculateOutputs(state);

	size_t index = std::max_element(actionMat.begin(), actionMat.end()) - actionMat.begin();

	lastAction(0, index) = 1.0;
	actions[index]();
}

void dqn::Agent::TakeAction(size_t index)
{
	lastAction = DMatrix{ 1, actions.size() };

	lastAction(0, index) = 1.0;
	actions[index]();
}
