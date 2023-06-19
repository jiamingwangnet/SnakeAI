#pragma once

#include <vector>
#include "Tile.h"
#include "Snake.h"
#include <GEngine_v0.1/Game.h>
#include <random>
#include <DeepQLearning/Matrix.h>
#include <fstream>

class Board
{
public:
	enum class State
	{
		PLAYING, LOSE
	};

	static constexpr double COLLIDE =  -1.0;
	static constexpr double EMPTY =     0.0;
	static constexpr double OBJECTIVE = 1.0;

private:
	struct DefaultState
	{
		int width;
		int height;
		int snakeLen;
		int delay;
	};

public:
	Board(GEngine::Game& game, int width, int height, int snakeLen, int delay);
	

	void Init();
	void Update();

	Snake& GetSnake() { return snake; }
	GEngine::Vector2& GetApple() { return applePos; }
	const int& GetScore() const { return score; }
	void SetState(State state) { this->state = state; }
	const State& GetState() { return state; }

	// parse board centered at snake position
		//		walls/snake body = -1
		//		apple            =  1
		//      empty            =  0
	dqn::DMatrix SampleBoard(int width, int height, int x, int y);
	dqn::DMatrix SampleBoard();
	const int& GetTravelledDistance() const { return travelledDistance; }

	bool isCollide(int x, int y)
	{
		return x >= width || y >= height || x < 0 || y < 0 ||
			tiles[y * width + x]->GetColor() == SNAKE_COLOR;
	}
	
	dqn::DMatrix GetAgentState()
	{
		dqn::DMatrix state{ 1, 11 };

		int bodyx = snake.GetBody()[0].x;
		int bodyy = snake.GetBody()[0].y;

		int foodx = applePos.x;
		int foody = applePos.y;

		GEngine::Vector2 next_n{ snake.GetBody()[0].x, snake.GetBody()[0].y - 1 };
		GEngine::Vector2 next_e{ snake.GetBody()[0].x + 1, snake.GetBody()[0].y };
		GEngine::Vector2 next_s{ snake.GetBody()[0].x, snake.GetBody()[0].y + 1 };
		GEngine::Vector2 next_w{ snake.GetBody()[0].x - 1, snake.GetBody()[0].y };

		bool dir_n = snake.GetHeading() == Snake::NORTH;
		bool dir_e = snake.GetHeading() == Snake::EAST;
		bool dir_s = snake.GetHeading() == Snake::SOUTH;
		bool dir_w = snake.GetHeading() == Snake::WEST;

		// collide straight
		state(0, 0) = (double)(dir_n && isCollide(next_n.x, next_n.y)) ||
					  (dir_e && isCollide(next_e.x, next_e.y)) ||
					  (dir_s && isCollide(next_s.x, next_s.y)) ||
					  (dir_w && isCollide(next_w.x, next_w.y));

		// collide right
		state(0, 1) = (double)(dir_n && isCollide(next_e.x, next_e.y)) ||
				      (dir_e && isCollide(next_s.x, next_s.y)) ||
				      (dir_s && isCollide(next_w.x, next_w.y)) ||
				      (dir_w && isCollide(next_n.x, next_n.y));

		// collide left
		state(0, 2) = (double)(dir_n && isCollide(next_w.x, next_w.y)) ||
					  (dir_e && isCollide(next_n.x, next_n.y)) ||
					  (dir_s && isCollide(next_e.x, next_e.y)) ||
					  (dir_w && isCollide(next_s.x, next_s.y));

		// move dir
		state(0, 3) = (double)(dir_n);
		state(0, 4) = (double)(dir_e);
		state(0, 5) = (double)(dir_s);
		state(0, 6) = (double)(dir_w);

		// food pos
		state(0, 7)  = (double)(foodx < bodyx); // west
		state(0, 8)  = (double)(foodx > bodyx); // east
		state(0, 9)  = (double)(foody < bodyy); // north
		state(0, 10) = (double)(foody > bodyy); // south

		return state;
	}

	void LogCSV()
	{
		std::ofstream csv{ "gameInfo.csv", std::ios::app };
		if(round == 0) csv << "round,score,time survived\n";
		csv << round << ',' << score << ',' << roundSteps << '\n';
		csv.close();
	}

	int GetRounds() const { return round; }

	void Reset();
private:
	std::vector<Tile*> tiles;
	Snake snake;
	int delay;
	GEngine::Game& game;

	GEngine::Vector2 applePos{0.0f, 0.0f};
	GEngine::Vector2 lastApplePos{0.0f};
	GEngine::Vector2 lastSnakePos{0.0f};

	int width;
	int height;

	std::random_device rd;
	std::mt19937 rng;

	int score = 0;

	State state = State::PLAYING;

	DefaultState defaultState;

	static constexpr GEngine::Color EMPTY_COLOR{ 50, 50, 50, 255 };
	static constexpr GEngine::Color SNAKE_COLOR{ 0, 255, 0, 255 };
	static constexpr GEngine::Color APPLE_COLOR{ 255, 0, 0, 255 };

	static constexpr float TILE_SIZE = 20.0f;
	static constexpr float PADDING = 2.0f;

	int round = 0;
	int roundSteps = 0;

	int travelledDistance = 0; // for each apple only
};

