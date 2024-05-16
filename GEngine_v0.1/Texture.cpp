#include "Texture.h"
#include "GameObject.h"
#include "Game.h"
#include <iostream>
#include <algorithm>
#include <iterator>

GEngine::Texture::Texture(GameObject& holder, const Material& mat, const Color& color, float depth)
	: Component{ holder }, material{mat}, depth{depth}
{
	for (int i = 0; i < 4; i++)
		this->colors[i] = color;
}

GEngine::Texture::Texture(GameObject& holder, const Material& mat, Color colors[4], float depth)
	: Component{ holder }, material{ mat }, depth{ depth }
{
	for (int i = 0; i < 4; i++)
		this->colors[i] = colors[i];
}

GEngine::Texture::~Texture()
{}

void GEngine::Texture::VInit()
{}

void GEngine::Texture::VUpdate()
{
	GameObject& holder = GetHolder();

	sprite.depth = depth;
	sprite.material = material;

	sprite.bottomLeft.SetColor(colors[0]);
	sprite.bottomLeft.SetPosition(holder.GetPosition().x, holder.GetPosition().y);
	sprite.bottomLeft.SetUV(0.0f, 0.0f);

	sprite.bottomRight.SetColor(colors[1]);
	sprite.bottomRight.SetPosition(holder.GetPosition().x + holder.GetSize().x, holder.GetPosition().y);
	sprite.bottomRight.SetUV(1.0f, 0.0f);

	sprite.topLeft.SetColor(colors[2]);
	sprite.topLeft.SetPosition(holder.GetPosition().x, holder.GetPosition().y + holder.GetSize().y);
	sprite.topLeft.SetUV(0.0f, 1.0f);

	sprite.topRight.SetColor(colors[3]);
	sprite.topRight.SetPosition(holder.GetPosition().x + holder.GetSize().x, holder.GetPosition().y + holder.GetSize().y);
	sprite.topRight.SetUV(1.0f, 1.0f);
}

void GEngine::Texture::VDraw()
{
	GetHolder().GetGame().GetRenderer().AddSprite(sprite);
}