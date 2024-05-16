#pragma once

#include <SDL3/SDL.h>
#include <string>
#include "Color.h"

namespace GEngine
{
	enum WINDOWFLAGS
	{
		WIND_INVISIBLE  = 0b001, 
		WIND_FULLSCREEN = 0b010, 
		WIND_BORDERLESS = 0b100
	};

	struct WinInfo
	{
		std::string name;
		int width;
		int height;
		Color clear;
		unsigned int flags;
	};

	class Window
	{
	public:
		Window(const WinInfo& info);
		~Window();
		void Create();
		void SwapBuffer();
		
		int GetWidth() const { return winInfo.width; }
		int GetHeight() const { return winInfo.height; }
		std::string GetName() const { return winInfo.name; }
	private:
		WinInfo winInfo;
		SDL_Window* window;
	};
}

