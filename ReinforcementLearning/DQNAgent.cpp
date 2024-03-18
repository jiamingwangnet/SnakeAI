#include "DQNAgent.h"
#include "Utility.h"
#include "CostFuncs.h"

#ifdef DEBUG_LOG
#include <fstream>

#define FILEOUT "debug.txt"
#endif

dqn::DQNAgent::DQNAgent(const std::vector<Action>& actions, const std::vector<net::LayerInfo>& layerinfo, int replaySize, double epsilonDecay)
	: Agent{actions}, actionValueFunc{layerinfo}, targetActionValueFunc{actionValueFunc}, epsilonDecay{epsilonDecay}, replaySize{replaySize}
{}

void dqn::DQNAgent::SampleBegin(const net::DMatrix& state)
{
	using namespace net;

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

void dqn::DQNAgent::SampleEnd(const net::DMatrix& newState, double reward, bool done)
{
	using namespace net;

	replays.emplace_back();
	ReplayData& replay = replays.back();
	replay.action_t = lastAction;
	replay.state_t = lastState;
	replay.reward_t = reward;
	replay.done = done;

	replay.state_next = newState;

	if (replays.size() > replaySize)
		replays.pop_front();

	epsilon *= epsilonDecay;
}

void dqn::DQNAgent::Train()
{
	using namespace net;

	steps++;

	const ReplayData& replay = replays[Util::Random<size_t>(std::uniform_int_distribution<size_t>(0, replays.size() - 1))];

	DMatrix target = actionValueFunc.CalculateOutputs(replay.state_t);
	DMatrix newVal = targetActionValueFunc.CalculateOutputs(replay.state_next);

	size_t index = (size_t)(std::max_element(replay.action_t.begin(), replay.action_t.end()) - replay.action_t.begin());

	target(0, index)
		= replay.reward_t + discount * *std::max_element(newVal.begin(), newVal.end()) * (1.0 - (double)replay.done);

	MSE<double> mse;
	DMatrix output = actionValueFunc.CalculateOutputs(replay.state_t);

	DMatrix costGrad = mse.Derivative(output, target);

	actionValueFunc.GradientDescent(0.001, costGrad);

	if (steps % RESET_RATE == 0)
	{
		targetActionValueFunc = actionValueFunc;
	}
}


void dqn::DQNAgent::TrainBatch()
{
	throw std::exception{ "Not Implemented" };
}

void dqn::DQNAgent::TakeAction(const net::DMatrix& state)
{
	using namespace net;

	lastAction = DMatrix{ 1, actions.size() };

	DMatrix actionMat = actionValueFunc.CalculateOutputs(state);

	size_t index = std::max_element(actionMat.begin(), actionMat.end()) - actionMat.begin();

	lastAction(0, index) = 1.0;
	actions[index]();
}

void dqn::DQNAgent::TakeAction(size_t index)
{
	using namespace net;

	lastAction = DMatrix{ 1, actions.size() };

	lastAction(0, index) = 1.0;
	actions[index]();
}
