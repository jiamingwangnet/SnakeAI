#pragma once

#include "MaterialCache.h"
#include "TextureCache.h"
#include <string>

namespace GEngine
{
	class ResourceManager
	{
	public:
		static Material GetMaterial(const std::string& texturePath, const std::string& vertShaderPath, const std::string& fragShaderPath);
		static GLTexture GetTexture(const std::string& texturePath);
	private:
		static TextureCache textureCache;
		static MaterialCache materialCache;
	};
}

