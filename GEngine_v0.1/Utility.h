#pragma once

#include <functional>
#include "GLTexture.h"
#include "MaterialCache.h"

namespace GEngine
{
	inline std::size_t CombineHash(std::size_t a, std::size_t b)
	{
		return a ^ (b + 0x9e3779b9 + (a << 6) + (a >> 2));
	}
}

template <>
struct std::hash<GEngine::GLTexture>
{
	std::size_t operator()(GEngine::GLTexture const& texture)
	{
		std::size_t idHash = std::hash<GLuint>{}(texture.id);
		std::size_t widthHash = std::hash<GLuint>{}(texture.width);
		std::size_t heightHash = std::hash<GLuint>{}(texture.height);

		return GEngine::CombineHash(idHash, GEngine::CombineHash(widthHash, heightHash));
	}
};

// create a hash for material for comparison
template <>
struct std::hash<GEngine::Material>
{
	std::size_t operator()(GEngine::Material const& material)
	{
		std::size_t textureHash = std::hash<GEngine::GLTexture>{}(material.texture);
		std::size_t progIDHash = std::hash<GLuint>{}(material.programID);

		return GEngine::CombineHash(textureHash, progIDHash);
	}
};