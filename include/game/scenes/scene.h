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
	//cursed_engine::EntityLoader 
	cursed_engine::SystemManager* systemManager;// Or each scene stores its own system manager??
};

class SceneLoader;

class Scene
{
public:
	Scene(cursed_engine::SystemManager* systemManager, std::string id);
	virtual ~Scene() = default;

	void update(float deltaTime); // or base scene updates ecs?
	virtual void onUpdate(float deltaTime) = 0;

	inline std::string_view id() const { return m_id; }

	virtual void onEnter() {};
	virtual void onExit() {};

	virtual void onCreated() {};
	virtual void onDestroyed() {};

private:
	friend class SceneLoader;

	ECSContext m_context;
	std::string m_id;
};