#include "BoxCollider.h"
#include "Vector.h"
#include "GameObject.h"
#include <vector>

std::vector<GEngine::GameObject*> GEngine::BoxCollider::colliders;

GEngine::BoxCollider::BoxCollider(GameObject& holder, bool isStatic)
	: Component{holder}, isStatic{isStatic}
{
	colliders.push_back(&holder);
}

GEngine::BoxCollider::~BoxCollider()
{
}

void GEngine::BoxCollider::VInit()
{
}

void GEngine::BoxCollider::VUpdate()
{
	GameObject& holder = GetHolder();

	const Vector2& holderP = holder.GetPosition();
	const Vector2& holderS = holder.GetSize();

	for (GameObject* otherGobj : colliders)
	{
		if (isStatic) continue;

		const Vector2& otherP = otherGobj->GetPosition();
		const Vector2& otherS = otherGobj->GetSize();

		const bool xCol = holderP.x < otherP.x + otherS.x && holderP.x + holderS.x > otherP.x;
		const bool yCol = holderP.y < otherP.y + otherS.y && holderP.y + holderS.y > otherP.y;

		// collide this.top and other.bottom
		const bool top    = (holderP.y + holderS.y > otherP.y && holderP.y < otherP.y && xCol);

		// collide this.right and other.left
		const bool right  = (holderP.x + holderS.x > otherP.x && holderP.x < otherP.x && yCol);

		// collide this.bottom and other.top
		const bool bottom = (holderP.y < otherP.y + otherS.y && holderP.y + holderS.y > otherP.y + otherS.y && xCol);
		
		// collide this.left and other.right
		const bool left   = (holderP.x < otherP.x + otherS.x && holderP.x + holderS.x > otherP.x + otherS.x && yCol);

		struct
		{
			float top;
			float right;
			float bottom;
			float left;
		} distances;

		distances.top    = std::abs((holderP.y + holderS.y) - otherP.y);
		distances.right  = std::abs((holderP.x + holderS.x) - otherP.x);
		distances.bottom = std::abs((otherP.y + otherS.y) - holderP.y);
		distances.left   = std::abs((otherP.x + otherS.x) - holderP.x);

		if (top && distances.top < distances.left && distances.top < distances.right)
		{
			holder.SetPosition(Vector2{holderP.x, otherP.y - holderS.y });
		}

		if (right && distances.right < distances.top && distances.right < distances.bottom)
		{
			holder.SetPosition(Vector2{ otherP.x - holderS.x, holderP.y });
		}

		if (bottom && distances.bottom < distances.left && distances.bottom < distances.right)
		{
			holder.SetPosition(Vector2{ holderP.x, otherP.y + otherS.y });
		}

		if (left && distances.left < distances.top && distances.left < distances.bottom)
		{
			holder.SetPosition(Vector2{ otherP.x + otherS.x, holderP.y });
		}
	}
}

void GEngine::BoxCollider::VDraw()
{
}

