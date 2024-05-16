#pragma once

#include <unordered_map>
#include <string>
#include "GLTexture.h"

namespace GEngine
{
	class TextureCache
	{
	public:
		TextureCache();
		~TextureCache();

		GLTexture GetTexture(const std::string& path);
	private:
		std::unordered_map<std::string, GLTexture> cache;
	};
}

