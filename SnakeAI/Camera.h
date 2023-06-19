#pragma once

#include <GEngine_v0.1/Camera2D.h>
#include <GEngine_v0.1/GameObject.h>

class Camera : public GEngine::GameObject
{
public:
	Camera(GEngine::Game& game, GEngine::Vector2 position)
		: GEngine::GameObject{game, position, GEngine::Vector2{1.0f, 1.0f}}
	{}

protected:
	void VInit() override
	{
		AddComponent<GEngine::Camera2D>(1280, 800);
	}

	void VUpdate() override {}
	void VDraw() override {}
};

