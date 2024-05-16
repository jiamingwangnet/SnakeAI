#pragma once

#include <unordered_map>
#include <SDL3/SDL.h>
#include "Vector.h"

namespace GEngine
{
	using ButtonCode = unsigned int;

	class InputManager
	{
	public:
		InputManager();
		~InputManager();

		void ProcessInput();
		bool HasQuit() const { return quitEvent; };
		Vector2 GetMousePosition() const { return mousePosition; }
		bool IsDown(ButtonCode btn) const;
	private:
		void PressButton(ButtonCode btn);
		void ReleaseButton(ButtonCode btn);
		void SetMouseCoords(float x, float y);
	private:
		std::unordered_map<ButtonCode, bool> keyMap;
		Vector2 mousePosition;
		bool quitEvent = false;
	};
}
