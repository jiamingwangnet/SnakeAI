#pragma once

#include "Color.h"

namespace GEngine
{
	struct Vertex
	{
		struct Position
		{
			float x;
			float y;
		} position;
		Color color;
		struct UV
		{
			float u;
			float v;
		} uv;

		void SetPosition(float x, float y)
		{
			position.x = x;
			position.y = y;
		}

		void SetColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
		{
			color.r = r;
			color.g = g;
			color.b = b;
			color.a = a;
		}

		void SetColor(const Color& color)
		{
			SetColor(color.r, color.g, color.b, color.a);
		}

		void SetUV(float u, float v)
		{
			uv.u = u;
			uv.v = v;
		}
	};
}