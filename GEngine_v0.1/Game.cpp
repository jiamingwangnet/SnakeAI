#include "Game.h"
#include <SDL3/SDL.h>
#include <GL/glew.h>
#include <iostream>

GEngine::Game::Game(const WinInfo& wininfo, float targetFPS)
	: window{wininfo}, state{GSTATE::PLAY}, renderer{*this}, mainCamera{nullptr}, fpsManager{targetFPS}, targetFPS{ targetFPS }
{

}

GEngine::Game::~Game()
{}

void GEngine::Game::SetFunctions(gfunc init, gfunc update, gfunc draw)
{
	finit = init;
	fupdate = update;
	fdraw = draw;
}

void GEngine::Game::Run()
{
	Init();
	GameLoop();
}

void GEngine::Game::Init()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	window.Create();
	renderer.Init();

	// don't init gameObjects because they they initalised when created

	finit();
}

void GEngine::Game::GameLoop()
{
	static float targetFrameTime = 1000.0f / targetFPS;

	while (state != GSTATE::EXIT)
	{
		fpsManager.BeginLimiter();

		input.ProcessInput();

		if (input.HasQuit()) state = GSTATE::EXIT;

		float frameLag = std::max(fpsManager.GetFrameTime() / targetFrameTime, 1.0f);

		for (int i = 0; i < (int)std::ceil(frameLag); i++)
		{
			Update();
			frames++;
		}
		if(render) Draw();

		fps = fpsManager.EndLimiter();

		

		if (frames % 100 == 0)
			std::cout << "FPS: " << fps << "\tFrame time: " << fpsManager.GetFrameTime() << "ms" << '\n';
	}
}

void GEngine::Game::Update()
{
	for (std::unique_ptr<GameObject>& gameObject : gameObjects)
	{
		gameObject->Update();
	}
	fupdate();
}

void GEngine::Game::Draw()
{
	if (mainCamera == nullptr)
		throw std::exception{ "Main camera not set." };

	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	renderer.Begin(SPRITESORT::FRONT_TO_BACK); // TODO: add option to change this
	for (std::unique_ptr<GameObject>& gameObject : gameObjects)
	{
		gameObject->Draw();
	}
	fdraw();
	renderer.End();
	renderer.Render();

	window.SwapBuffer();
}
