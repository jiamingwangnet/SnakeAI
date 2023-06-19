#pragma once

#include <GEngine_v0.1/GameObject.h>
#include <GEngine_v0.1/Color.h>
#include <GEngine_v0.1/Texture.h>

class Tile : public GEngine::GameObject
{
public:
	Tile(GEngine::Game& game, GEngine::Vector2 position, GEngine::Vector2 size, const GEngine::Color& color);

	void SetColor(const GEngine::Color& color) { 
		this->color = color; 
		GetComponent<GEngine::Texture>()->SetColor(color);
	}
	const GEngine::Color& GetColor() const { return color; }

protected:
	void VInit() override;
	void VUpdate() override;
	void VDraw() override;
	
private:
	GEngine::Color color;
};

