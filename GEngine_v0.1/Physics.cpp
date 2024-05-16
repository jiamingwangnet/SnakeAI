#include "Physics.h"
#include "GameObject.h"

GEngine::Physics::Physics(GameObject& holder, float mass, float friction)
	: Component{holder}, mass{ mass }, friction{ friction }, velocity{ 0.0f, 0.0f }
{
}

GEngine::Physics::~Physics()
{
}

void GEngine::Physics::AddForce(Vector2 force)
{
	velocity += force * time / mass;
}

void GEngine::Physics::VInit()
{
}

void GEngine::Physics::VUpdate()
{
	GameObject& holder = GetHolder();
	holder.SetPosition(holder.GetPosition() + velocity);

	ApplyFriction();
}

void GEngine::Physics::VDraw()
{
}

void GEngine::Physics::ApplyFriction()
{
	if (velocity.x != 0)
	{
		velocity.x += friction * (0 - velocity.x);
	}

	if (velocity.y != 0)
	{
		velocity.y += friction * (0 - velocity.y);
	}
}
