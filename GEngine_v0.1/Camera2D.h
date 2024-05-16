#pragma once

#include "Component.h"
#include "Vector.h"
#include "Matrix.h"

namespace GEngine
{
	class Camera2D : public Component
	{
	public:
		Camera2D(GameObject& holder, int scrnWidth, int scrnHeight);
		~Camera2D();

		Vector2 ScreenToWorldCoords(Vector2 screenCoords);
		Matrix4x4 GetMatrix() const { return cameraMatrix; }
	protected:
		void VInit() override;
		void VUpdate() override;
		void VDraw() override;
	private:
		int scrnWidth;
		int scrnHeight;
		float scale;

		Matrix4x4 cameraMatrix;
		Matrix4x4 orthoMatrix;

		bool needUpdate;
	};
}
