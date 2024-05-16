#include "GameObject.h"
GEngine::GameObject::GameObject(Game& game, Vector2 position, Vector2 size)
	: game{game}, position{position}, size{size}
{
	
}

GEngine::GameObject::~GameObject()
{}

void GEngine::GameObject::Init()
{
	for (std::unique_ptr<Component>& cmp : components)
	{
		cmp->Init();
	}
	VInit();
}

void GEngine::GameObject::Update()
{
	for (std::unique_ptr<Component>& cmp : components)
	{
		cmp->Update();
	}
	VUpdate();
}

void GEngine::GameObject::Draw()
{
	for (std::unique_ptr<Component>& cmp : components)
	{
		cmp->Draw();
	}
	VDraw();
}