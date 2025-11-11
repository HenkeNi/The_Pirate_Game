#pragma once
#include "engine/ecs/ecs_registry.h"
#include <vector>

namespace cursed_engine
{
	class SystemManager;
}

struct ECSContext
{
	ECSContext(cursed_engine::SystemManager* manager)
		: systemManager{ manager }
	{
	}

	//EntityFactory
	cursed_engine::ECSRegistry registry;
	cursed_engine::SystemManager* systemManager;// Or each scene stores its own system manager??
};

class Scene
{
public:
	Scene(cursed_engine::SystemManager* systemManager);
	virtual ~Scene() = default;

	void update(float deltaTime); // or base scene updates ecs?
	virtual void onUpdate(float deltaTime) = 0;

	virtual void onEnter() {};
	virtual void onExit() {};

	virtual void onCreated() {};
	virtual void onDestroyed() {};

private:
	ECSContext m_context;
};