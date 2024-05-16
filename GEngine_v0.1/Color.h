#pragma once

#include <GL/glew.h>

namespace GEngine
{
	struct Color
	{
		GLubyte r;
		GLubyte g;
		GLubyte b;
		GLubyte a;

		bool operator==(const Color& rhs) const
		{
			return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a;
		}

		bool operator!=(const Color& rhs) const
		{
			return !(*this == rhs);
		}
	};
}
