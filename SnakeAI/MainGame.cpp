#include "MainGame.h"
#include "Camera.h"
#include <iostream>
#include <DeepQLearning/Utility.h>

static constexpr int DELAY = 1;
static constexpr int VIEW_SIZE = 10;
static constexpr int PENALTY_DISTANCE = 200; // if the agent stays in one place for too long or travels to far give a negative reward
static constexpr double PENALTY = -5.0; // the penalty to give

MainGame::MainGame(GEngine::Game& game)
	: game{ game }, board{ game, 32, 24, 3, DELAY },
	actions{
		// straight
		[this]() {
			// do nothing
		},
		
		// left
		[this]() {
			if (this->GetBoard().GetSnake().GetHeading() == Snake::NORTH)
			{
				this->GetBoard().GetSnake().SetHeading(Snake::WEST);
			}
			else if (this->GetBoard().GetSnake().GetHeading() == Snake::EAST)
			{
				this->GetBoard().GetSnake().SetHeading(Snake::NORTH);
			}
			else if (this->GetBoard().GetSnake().GetHeading() == Snake::SOUTH)
			{
				this->GetBoard().GetSnake().SetHeading(Snake::EAST);
			}
			else if (this->GetBoard().GetSnake().GetHeading() == Snake::WEST)
			{
				this->GetBoard().GetSnake().SetHeading(Snake::SOUTH);
			}
		},

		// right
		[this]() {
			if (this->GetBoard().GetSnake().GetHeading() == Snake::NORTH)
			{
				this->GetBoard().GetSnake().SetHeading(Snake::EAST);
			}
			else if (this->GetBoard().GetSnake().GetHeading() == Snake::EAST)
			{
				this->GetBoard().GetSnake().SetHeading(Snake::SOUTH);
			}
			else if (this->GetBoard().GetSnake().GetHeading() == Snake::SOUTH)
			{
				this->GetBoard().GetSnake().SetHeading(Snake::WEST);
			}
			else if (this->GetBoard().GetSnake().GetHeading() == Snake::WEST)
			{
				this->GetBoard().GetSnake().SetHeading(Snake::NORTH);
			}
		},
	},
	layerInfo{
		{ 11,     0, &relu},
		{ 256,   11, &relu},
		//{ 128,  256, &relu},
		{   3,  256, &linear}
	},
	agent{actions, layerInfo, 100000, 0.9998}
{
	
}

void MainGame::Init()
{
	game.SetMainCamera(game.CreateGameObject<Camera>(GEngine::Vector2{0.0f, 0.0f}));

	board.Init();

	std::cout << "Save file: ";
	std::cin >> name;
	std::ifstream in{ name + ".actvalue", std::ios::binary};
	if (in.good())agent.Load("", name);
}

void MainGame::Update()
{
	if (game.GetFrames() % DELAY == 0)
	{
		dqn::DMatrix state_0 = board.GetAgentState();

		double prevScore = (double)board.GetScore();

		agent.SampleBegin(state_0);

		board.Update();

		dqn::DMatrix state_1 = board.GetAgentState();

		//double multiplier = 1.0; // multiply reward??

		//double penalty = 0.0; // calculate distance penalty
		/*if (board.GetTravelledDistance() >= PENALTY_DISTANCE) 
			penalty = PENALTY;*/

		if (board.GetState() == Board::State::LOSE)
		{
			//agent.SampleEnd(state_1, -10.0 + penalty, true);
			agent.SampleEnd(state_1, -10.0, true);
			//multiplier = 1.0;

			//agent.TrainBatch();
		}
		else
		{
			//agent.SampleEnd(state_1, multiplier * 10.0 * ((double)board.GetScore() - prevScore) + penalty, false);
			agent.SampleEnd(state_1, 10.0 * ((double)board.GetScore() - prevScore), false);

			//multiplier += 0.5;

			//agent.Train();
		}
		/*agent.TrainBatch();*/
		agent.Train();

		if (board.GetRounds() % 20 == 0)
		{
			agent.Save("", name);
		}
	}
	

	if (board.GetState() == Board::State::LOSE)
	{
		board.Reset();
	}
}

void MainGame::Draw()
{
}
