#pragma once

#include "Lib/picopng.h"
#include "GLTexture.h"
#include <string>

namespace GEngine
{
	class ImagerLoader
	{
	public:
		static GLTexture LoadPNG(std::string path);
	};
}
