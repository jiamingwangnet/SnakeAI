#include "Renderer.h"
#include <algorithm>
#include "Utility.h"
#include "Game.h"

GEngine::Renderer::Renderer(const Game& game)
	: vbo{0}, vao{0}, sortType{SPRITESORT::NONE}, game{game}
{}

GEngine::Renderer::~Renderer()
{}

void GEngine::Renderer::Init()
{
	// create vertex array object (vao)
	if (vao == 0) glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	if (vbo == 0) glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// attribute pointers, the shaders must have the attributes in the same order
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	glBindVertexArray(0); // unbind
}

void GEngine::Renderer::Begin(SPRITESORT sort)
{
	sortType = sort;
	batches.clear();
	sprites.clear();
}

void GEngine::Renderer::End()
{
	SortSprites();
	CreateBatches();
}

void GEngine::Renderer::Render()
{
	glBindVertexArray(vao);
	glActiveTexture(GL_TEXTURE0);

	size_t i = 0;
	for (const RenderBatch& batch : batches)
	{
		if (i == 0 || batch.material != batches[i - 1].material) // keep using the same shader if two objects share the same material
		{
			batch.material.UseShader();
			batch.material.SetUniform("textureSampler", 0);
			batch.material.SetUniform("frames", (float)game.GetFrames());
			batch.material.SetUniform("cameraMatrix", game.GetMainCamera()->GetComponent<Camera2D>()->GetMatrix());
		}

		glBindTexture(GL_TEXTURE_2D, batch.material.texture.id);
		glDrawArrays(GL_TRIANGLES, batch.offset, batch.nVerts);

		if (i + 1 == batches.size() || batch.material != batches[i + 1].material)
			batch.material.UnuseShader();
		i++;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0); // unbind
}

void GEngine::Renderer::AddSprite(const Sprite& sprite)
{
	sprites.push_back(std::make_unique<Sprite>(sprite));
}

void GEngine::Renderer::SortSprites()
{
	using SpritePtr = std::unique_ptr<Sprite>;

	switch (sortType)
	{
	case SPRITESORT::FRONT_TO_BACK:
		std::stable_sort(sprites.begin(), sprites.end(), [](const SpritePtr& a, const SpritePtr& b) { return a->depth < b->depth; });
		break;
	case SPRITESORT::BACK_TO_FRONT:
		std::stable_sort(sprites.begin(), sprites.end(), [](const SpritePtr& a, const SpritePtr& b) { return a->depth > b->depth; });
		break;
	case SPRITESORT::TEXTURE: // compare hashes to make sure the same materials are placed next to each other
		std::stable_sort(sprites.begin(), sprites.end(), [](const SpritePtr& a, const SpritePtr& b) { return std::hash<Material>{}(a->material) < std::hash<Material>{}(b->material); });
		break;
	}
}

void GEngine::Renderer::CreateBatches()
{
	if (sprites.empty()) return;

	std::vector<Vertex> vertices;
	vertices.resize(sprites.size() * 6); // 6 vertices per sprite

	int vert = 0;
	GLuint offset = 0;

	batches.emplace_back<RenderBatch>({ offset, 6, sprites[0]->material });
	vertices[vert++] = sprites[0]->topRight;
	vertices[vert++] = sprites[0]->topLeft;
	vertices[vert++] = sprites[0]->bottomLeft;
	vertices[vert++] = sprites[0]->bottomLeft;
	vertices[vert++] = sprites[0]->bottomRight;
	vertices[vert++] = sprites[0]->topRight;
	offset += 6;

	for (int sprite = 1; sprite < sprites.size(); sprite++, offset += 6)
	{
		if (sprites[sprite]->material != sprites[sprite - 1]->material) // if the current sprite is different from the last sprite, put it into a new batch
			batches.emplace_back<RenderBatch>({ offset, 6, sprites[sprite]->material });
		else
			batches.back().nVerts += 6;
		vertices[vert++] = sprites[sprite]->topRight;
		vertices[vert++] = sprites[sprite]->topLeft;
		vertices[vert++] = sprites[sprite]->bottomLeft;
		vertices[vert++] = sprites[sprite]->bottomLeft;
		vertices[vert++] = sprites[sprite]->bottomRight;
		vertices[vert++] = sprites[sprite]->topRight;
	}

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW); // orphan buffer
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data()); // upload the vertices
	glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind
}
