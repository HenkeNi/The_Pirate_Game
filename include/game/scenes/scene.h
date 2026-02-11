#pragma once
#include "engine/ecs/ecs_registry.h"
#include "engine/core/registry_aliases.h"
#include <vector>

namespace cursed_engine
{
	class SystemManager;
	class EntityFactory;

	//template <typename T, typename ID>
	//class TypeRegistry; // TODO; replace with type alias for ComponentInfo instead!!

	//struct ComponentInfo;
}

struct SceneContext
{
	SceneContext(cursed_engine::SystemManager* manager, cursed_engine::EntityFactory* factory, cursed_engine::ComponentRegistry* componentData_)
		: systemManager{ manager }, entityFactory{ factory }, componentData{ componentData_ }
	{
	}

	//EntityFactory
	cursed_engine::ECSRegistry registry;
	//cursed_engine::EntityLoader 
	cursed_engine::SystemManager* systemManager;// Or each scene stores its own system manager??
	cursed_engine::EntityFactory* entityFactory;
	
	cursed_engine::TypeRegistry<cursed_engine::ComponentInfo, uint32_t>* componentData; //  todo, accept this in constructor...
	//Todo, store meta registry (component data)
};

class SceneLoader;

class Scene
{
public:
	// TODO; fix this...
	Scene(cursed_engine::SystemManager* systemManager, cursed_engine::EntityFactory* entityFactory, cursed_engine::ComponentRegistry* componentData, std::string id);
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

	SceneContext m_context;
	std::string m_id;
};