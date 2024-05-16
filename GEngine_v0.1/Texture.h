#pragma once
#include "Component.h"
#include "MaterialCache.h"
#include "Renderer.h"
#include <string>

namespace GEngine
{
	class Texture : public Component
	{
	public:
		Texture(GameObject& holder, const Material& mat, const Color& color, float depth = 0.0f);
		Texture(GameObject& holder, const Material& mat, Color colors[4], float depth = 0.0f);
		~Texture();

		void SetColor(const Color& color)
		{
			for (int i = 0; i < 4; i++)
				this->colors[i] = color;
		}
		void SetColor(Color colors[4])
		{
			for (int i = 0; i < 4; i++)
				this->colors[i] = colors[i];
		}
	protected:
		void VInit() override;
		void VUpdate() override;
		void VDraw() override;
	private:
		Material material;
		Sprite sprite;
		Color colors[4];
		float depth;
	};
}

