#pragma once

#include "Component.h"
#include "Vector.h"
#include <vector>
#include <memory>

namespace GEngine
{
	class Game;

	class GameObject
	{
	protected:
		GameObject(Game& game, Vector2 position, Vector2 size);
	public:
		virtual ~GameObject();

		// used by the Game class for the game cycle
		void Init();
		void Update();
		void Draw();

		const Vector2& GetPosition() const { return position; }
		void SetPosition(const Vector2& position) { this->position = position; }

		const Vector2& GetSize() const { return size; }
		void SetSize(const Vector2& size) { this->size = size; }

		Game& GetGame() { return game; }

		template<class T>
		T* GetComponent() // change to shared_ptr??
		{
			for (std::unique_ptr<Component>& cmp : components)
			{
				if (typeid(T) == typeid(*(cmp.get())))
				{
					return static_cast<T*>(cmp.get());
				}
			}
			return nullptr;
		}

		template<class T, typename... Args>
		T* AddComponent(Args&&... args) // return T * for less type casting
		{
			// check if T is a derived class of Component?

			components.push_back(std::make_unique<T>(*this, std::forward<Args>(args)...));
			components.back()->Init();
			return static_cast<T*>(components.back().get());
		}

	protected:
		// overriden by the child class
		virtual void VInit() = 0;
		virtual void VUpdate() = 0;
		virtual void VDraw() = 0;

	private:
		std::vector<std::unique_ptr<Component>> components;
		Vector2 position;
		Vector2 size;
		Game& game;
	};
}

