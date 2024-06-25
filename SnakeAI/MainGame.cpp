#include "MainGame.h"
#include "Camera.h"
#include <iostream>
#include <ReinforcementLearning/Utility.h>

static constexpr int DELAY = 1;
static constexpr int VIEW_SIZE = 10;
static constexpr int SNAKE_LEN = 10;

MainGame::MainGame(GEngine::Game& game)
	: game{ game }, board{ game, 32, 24, SNAKE_LEN, DELAY },
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
		{ 11,     0, relu},
		{ 256,   11, relu},
		{ 128,  256, relu},
		{   3,  128, linear}
	},
#ifdef PPO
	agent{actions, layerInfo, 100000}
#else
	agent{actions, layerInfo, 100000, 0.9998}
#endif
	, display{ game, agent, -620.0f, -250.0f, 2.0f }
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

	display.Init();
}

void MainGame::Update()
{
	if (game.GetFrames() % DELAY == 0)
	{
		net::DMatrix state_0 = board.GetAgentState();

		double prevScore = (double)board.GetScore();

		agent.SampleBegin(state_0);

		board.Update();

		display.Update();

		net::DMatrix state_1 = board.GetAgentState();

		if (board.GetState() == Board::State::LOSE)
		{
			agent.SampleEnd(state_1, -10.0, true);
		}
		else
		{
			agent.SampleEnd(state_1, 10.0 * ((double)board.GetScore() - prevScore), false);
		}

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
