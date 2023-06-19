#include "Board.h"

Board::Board(GEngine::Game& game, int width, int height, int snakeLen, int delay)
	: snake{snakeLen, width / 2, height / 2 }, delay{delay}, game{game}, width{width}, height{height}, rng{rd()}, defaultState{}
{
	defaultState.delay = delay;
	defaultState.width = width;
	defaultState.height = height;
	defaultState.snakeLen = snakeLen;
}

void Board::Init()
{
	tiles.resize(width * height);
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			tiles[y * width + x] = game.CreateGameObject<Tile>(
				GEngine::Vector2{
					x * (TILE_SIZE + PADDING) - 1280.0f/2.0f + (1280.0f - (TILE_SIZE + PADDING) * width) / 2.0f,
					y * (TILE_SIZE + PADDING) - 800.0 / 2.0f + (800.0 - (TILE_SIZE + PADDING) * height) / 2.0f
				}, 
				GEngine::Vector2{ TILE_SIZE, TILE_SIZE }, EMPTY_COLOR);
		}
	}

	std::uniform_int_distribution<int> widthDist(0, width - 1);
	std::uniform_int_distribution<int> heightDist(0, height - 1);

	applePos = GEngine::Vector2{ widthDist(rng), heightDist(rng) };
	//applePos = GEngine::Vector2{ 5, 5 }; // static environment
}

void Board::Update()
{
	if (state == State::LOSE) return;

	if (game.GetFrames() % delay == 0)
	{
		roundSteps++;
		std::vector<Snake::Section>& body = snake.GetBody();

		tiles[lastApplePos.y * width + lastApplePos.x]->SetColor(EMPTY_COLOR);
		tiles[lastSnakePos.y * width + lastSnakePos.x]->SetColor(EMPTY_COLOR);

		/*for (int i = 0; i < width * height; i++)
		{
			tiles[i]->SetColor(EMPTY_COLOR);
		}*/
		
		if (!snake.Collide(width, height, tiles, SNAKE_COLOR))
		{
			snake.Update();
			travelledDistance++;

			for (const Snake::Section& section : body)
			{
				tiles[section.y * width + section.x]->SetColor(SNAKE_COLOR);
			}

			if (body[0].x == applePos.x && body[0].y == applePos.y)
			{
				std::uniform_int_distribution<int> widthDist(0, width - 1);
				std::uniform_int_distribution<int> heightDist(0, height - 1);

				applePos = GEngine::Vector2{ widthDist(rng), heightDist(rng) };
				travelledDistance = 0;
				//applePos = GEngine::Vector2{ 5, 5 }; // static environment

				body.emplace_back();

				score++;
			}

			lastSnakePos.x = body.back().x;
			lastSnakePos.y = body.back().y;
		}
		else
		{
			SetState(State::LOSE);
			tiles[lastSnakePos.y * width + lastSnakePos.x]->SetColor(SNAKE_COLOR);
			
			LogCSV();

			round++;
		}

		tiles[applePos.y * width + applePos.x]->SetColor(APPLE_COLOR);

		lastApplePos = applePos;
	}
}

dqn::DMatrix Board::SampleBoard(int width, int height, int x, int y)
{
	dqn::DMatrix res{ 1, (size_t)width * height };

	int cornerX = x - width / 2;
	int cornerY = y - height / 2;

	for (int dy = 0; dy < height; dy++)
	{
		for (int dx = 0; dx < width; dx++)
		{
			if (cornerX + dx < 0 || cornerY + dy < 0 || cornerX + dx >= width || cornerY + dy >= height || tiles[(cornerY + dy) * width + cornerX + dx]->GetColor() == SNAKE_COLOR)
			{
				res(0, dy * width + dx) = COLLIDE;
			}
			else if (tiles[(cornerY + dy) * width + cornerX + dx]->GetColor() == EMPTY_COLOR)
			{
				res(0, dy * width + dx) = EMPTY;
			} 
			else if (tiles[(cornerY + dy) * width + cornerX + dx]->GetColor() == APPLE_COLOR)
			{
				res(0, dy * width + dx) = OBJECTIVE;
			}
		}
	}

	return res;
}

dqn::DMatrix Board::SampleBoard()
{
	dqn::DMatrix res{ 1, (size_t)width * height };

	size_t i = 0;
	for (Tile* tile : tiles)
	{
		if (tile->GetColor() == SNAKE_COLOR)
		{
			res(0, i) = COLLIDE;
		}
		else if (tile->GetColor() == EMPTY_COLOR)
		{
			res(0, i) = EMPTY;
		}
		else if (tile->GetColor() == APPLE_COLOR)
		{
			res(0, i) = OBJECTIVE;
		}
		i++;
	}
	return res;
}

void Board::Reset()
{
	delay = defaultState.delay;
	height = defaultState.height;
	width = defaultState.width;
	travelledDistance = 0;
	roundSteps = 0;
	score = 0;
	snake = Snake{ defaultState.snakeLen, width / 2, height / 2 };

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			tiles[y * width + x]->SetColor(EMPTY_COLOR);
		}
	}

	std::uniform_int_distribution<int> widthDist(0, width - 1);
	std::uniform_int_distribution<int> heightDist(0, height - 1);

	applePos = GEngine::Vector2{ widthDist(rng), heightDist(rng) };
	//applePos = GEngine::Vector2{ 5, 5 }; // static environment


	state = State::PLAYING;
}
