#include "FPSManager.h"
#include <SDL3/SDL.h>

GEngine::FPSManager::FPSManager(float targetFPS)
	: fps{ 0.0f }, frameTime{ 0.0f }, startTicks{ 0 }
{
	SetTargetFPS(targetFPS);
}

void GEngine::FPSManager::BeginLimiter()
{
	startTicks = SDL_GetTicks();
}

float GEngine::FPSManager::EndLimiter()
{
	CalculateFPS();

	float frameTicks = SDL_GetTicks() - startTicks;
	if (1000.0f / target > frameTicks)
	{
		SDL_Delay(1000.0f / target - frameTicks);
	}

	return fps;
}

void GEngine::FPSManager::CalculateFPS()
{
	static constexpr int NSAMPLES = 10;
	static float timeBuffer[NSAMPLES];
	static int frame = 0;

	static float prevTicks = (float)SDL_GetTicks();

	float currentTicks = SDL_GetTicks();

	frameTime = currentTicks - prevTicks;
	timeBuffer[frame % NSAMPLES] = frameTime;

	prevTicks = currentTicks;
	frame++;

	int count = NSAMPLES;
	if (frame < NSAMPLES)
	{
		count = frame;
	}

	float average = 0.0f;
	for (int i = 0; i < count; i++)
	{
		average += timeBuffer[i];
	}	
	average /= (float)count;

	if (average > 0)
		fps = 1000.0f / average;
	else
		fps = target;
}