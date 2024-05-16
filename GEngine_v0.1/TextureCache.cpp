#include "TextureCache.h"
#include "GLTexture.h"
#include "ImagerLoader.h"
#include <iostream>

GEngine::TextureCache::TextureCache()
{}

GEngine::TextureCache::~TextureCache()
{}

GEngine::GLTexture GEngine::TextureCache::GetTexture(const std::string& path)
{
	auto iter = cache.find(path);

	if (iter == cache.end())
	{
		GLTexture texture = ImagerLoader::LoadPNG(path);
		cache.insert({ path, texture });

		std::cout << "Cached new texture.\n";

		return texture;
	}

	std::cout << "Retrieved texture from cache.\n";

	return iter->second;
}
