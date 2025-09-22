#pragma once
#include <vector>

namespace cursed_engine
{
	class Scene
	{
	public:
		virtual ~Scene() = default;

		virtual void update(float deltaTime) = 0; // or base scene updates ecs?

		virtual void onEnter() {};
		virtual void onExit() {};

		virtual void onCreated() {};
		virtual void onDestroyed() {};

	private:
		// entity factory?
		// world?		
	};
}