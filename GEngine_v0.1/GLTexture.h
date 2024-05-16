#pragma once

#include <GL/glew.h>

namespace GEngine
{
	struct GLTexture
	{
		GLuint id;
		int width;
		int height;

		bool operator==(const GLTexture& rhs) const
		{
			return id == rhs.id && width == rhs.width && height == rhs.height;
		}
		bool operator!=(const GLTexture& rhs) const
		{
			return !(*this == rhs);
		}
	};
}