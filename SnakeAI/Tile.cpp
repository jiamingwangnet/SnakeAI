#include "Tile.h"
#include <GEngine_v0.1/ResourceManager.h>

Tile::Tile(GEngine::Game& game, GEngine::Vector2 position, GEngine::Vector2 size, const GEngine::Color& color)
	: GEngine::GameObject{game, position, size}, color{color}
{
}

void Tile::VInit()
{
	AddComponent<GEngine::Texture>(
		GEngine::ResourceManager::GetMaterial("Textures/Tile.png", "Shaders/defaultShader.vert", "Shaders/defaultShader.frag"),
		color
	);
}

void Tile::VUpdate()
{
}

void Tile::VDraw()
{
}
