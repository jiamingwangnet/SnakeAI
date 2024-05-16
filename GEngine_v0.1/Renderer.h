#pragma once

#include <GL/glew.h>
#include "Vertex.h"
#include "MaterialCache.h"
#include <vector>
#include <memory>

namespace GEngine
{
	enum class SPRITESORT
	{
		NONE, FRONT_TO_BACK, BACK_TO_FRONT, TEXTURE
	};

	struct Sprite
	{
		Material material;
		float depth;

		Vertex topLeft;
		Vertex topRight;
		Vertex bottomLeft;
		Vertex bottomRight;
	};

	struct RenderBatch
	{
		GLuint offset;
		GLuint nVerts;
		Material material;
	};

	class Game;

	class Renderer
	{
	public:
		Renderer(const Game& game);
		~Renderer();

		void Init();
		void Begin(SPRITESORT sort);
		void End();
		void Render();
		void AddSprite(const Sprite& sprite);
	private:
		void SortSprites();
		void CreateBatches();
	private:
		GLuint vbo;
		GLuint vao;

		SPRITESORT sortType;
		std::vector<std::unique_ptr<Sprite>> sprites; // use pointers for more memory efficiency
		std::vector<RenderBatch> batches;
		const Game& game;
	};
}

