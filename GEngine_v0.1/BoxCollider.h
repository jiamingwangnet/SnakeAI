#pragma once

#include "Component.h"
#include <vector>

namespace GEngine
{
	class BoxCollider : public Component
	{
	public:
		BoxCollider(GameObject& holder, bool isStatic);
		~BoxCollider();
	protected:
		void VInit() override;
		void VUpdate() override;
		void VDraw() override;
	private:
		static std::vector<GameObject*> colliders;
		bool isStatic;
	};
}

