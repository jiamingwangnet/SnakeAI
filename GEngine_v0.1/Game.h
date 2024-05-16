#pragma once

#include <functional>
#include "Window.h"
#include "InputManager.h"
#include "Renderer.h"
#include "GameObject.h"
#include "Camera2D.h"
#include "FPSManager.h"
#include <memory>
#include <vector>

namespace GEngine
{
	enum class GSTATE
	{
		PLAY, EXIT
	};

	class Game
	{
	private:
		using gfunc = std::function<void(void)>;
	public:
		Game(const WinInfo& wininfo, float targetFPS = 60.0f);
		~Game();
		void Run();
		void SetFunctions(gfunc init, gfunc update, gfunc draw);
		GameObject* const GetMainCamera() const { return mainCamera; }
		Renderer& GetRenderer() { return renderer; }
		void SetRender(bool render) { this->render = render; }
		void SetMainCamera(GameObject* camera) 
		{ if (camera->GetComponent<Camera2D>() == nullptr) throw std::exception{ "GameObject is not a Camera2D" }; mainCamera = camera; }
		int GetFrames() const { return frames; }
		const InputManager& GetInput() { return input; }
		
		template<class T, typename... Args>
		T* CreateGameObject(Args&&... args) // return T* for less type casting
		{
			// check if T is type GameObject??

			gameObjects.push_back(std::make_unique<T>(*this, std::forward<Args>(args)...));
			gameObjects.back()->Init();

			return static_cast<T*>(gameObjects.back().get());
		}
		float GetFrameTime() const { return fpsManager.GetFrameTime(); }
	private:
		void Init();
		void GameLoop();
		void Update();
		void Draw();
	private:
		int frames = 0;

		Window window;
		InputManager input;
		Renderer renderer;
		FPSManager fpsManager;

		GSTATE state;

		gfunc finit;
		gfunc fupdate;
		gfunc fdraw;

		GameObject* mainCamera;
		std::vector<std::unique_ptr<GameObject>> gameObjects; // make sure objects in vectors are stored as pointers so the addresses dont change when the vector expands

		float fps = 0.0f;
		float targetFPS;
		bool render = true;
	};
}

