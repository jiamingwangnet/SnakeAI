#pragma once

#include "Component.h"
#include "Vector.h"

namespace GEngine
{
	class Physics : public Component
	{
	public:
		Physics(GameObject& holder, float mass, float friction);
		~Physics();

		void AddForce(Vector2 force);

	protected:
		void VInit() override;
		void VUpdate() override;
		void VDraw() override;

	private:
		void ApplyFriction();

	private:
		float mass;
		float friction;
		Vector2 velocity;

		static constexpr float time = 1.0f / 60.0f; // the time use for physics is the duration of one frame (assuming the game runs at 60fps)
	};

}