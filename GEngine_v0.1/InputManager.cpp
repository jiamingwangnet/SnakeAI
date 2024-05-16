#include "InputManager.h"

GEngine::InputManager::InputManager()
	: keyMap{}, mousePosition{0.0f, 0.0f}
{}

GEngine::InputManager::~InputManager()
{}

void GEngine::InputManager::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_EVENT_QUIT:
			quitEvent = true;
			break;
		case SDL_EVENT_MOUSE_MOTION:
			SetMouseCoords(event.motion.x, event.motion.y);
			break;
		case SDL_EVENT_KEY_DOWN:
			PressButton(event.key.keysym.sym);
			break;
		case SDL_EVENT_KEY_UP:
			ReleaseButton(event.key.keysym.sym);
			break;
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
			PressButton(event.button.button);
			break;  
		case SDL_EVENT_MOUSE_BUTTON_UP:
			ReleaseButton(event.button.button);
			break;
		}
	}
}

bool GEngine::InputManager::IsDown(ButtonCode btn) const
{
	auto it = keyMap.find(btn);
	if (it != keyMap.end())
		return it->second;
	return false;
}

void GEngine::InputManager::PressButton(ButtonCode btn)
{
	keyMap[btn] = true;
}

void GEngine::InputManager::ReleaseButton(ButtonCode btn)
{
	keyMap[btn] = false;
}

void GEngine::InputManager::SetMouseCoords(float x, float y)
{
	mousePosition.x = x;
	mousePosition.y = y;
}
