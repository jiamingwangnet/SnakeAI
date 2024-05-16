#pragma once

#include <string>

namespace GEngine
{
	class GameObject;
	class Component
	{
	protected:
		Component(GameObject& holder);
	public:
		virtual ~Component();

		// used by the GameObject class for the game cycle
		void Init();
		void Update();
		void Draw();

		bool IsEnabled() const { return enabled; }
		void SetEnabled(bool x) { enabled = x; }

		GameObject& GetHolder() const { return holder; }

	protected:
		// overriden by the child class
		virtual void VInit() = 0;
		virtual void VUpdate() = 0;
		virtual void VDraw() = 0;
	private:
		GameObject& holder;
		bool enabled;
	};
}

