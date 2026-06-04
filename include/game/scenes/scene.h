#pragma once
#include <engine/ecs/ecs_registry.h>
#include <vector>

#include <engine/ecs/component/component_registry.h>

namespace cursed_engine
{
	class SystemManager;
	class EntityFactory;
	class EventBus;
	//template <typename T, typename ID>
	//class TypeRegistry; // TODO; replace with type alias for ComponentInfo instead!!

	class ComponentRegistry;
	//struct ComponentInfo;
}

struct SceneData
{

};

struct SceneContext
{
	SceneContext(cursed_engine::SystemManager* manager, cursed_engine::EntityFactory* factory, cursed_engine::ComponentRegistry* componentRegistry, cursed_engine::EventBus* eventBus)
		: systemManager{ manager }, entityFactory{ factory }, componentRegistry{ componentRegistry }, eventBus{ eventBus }
	{
	}

	//EntityFactory
	cursed_engine::ECSRegistry registry;
	//cursed_engine::EntityLoader 
	cursed_engine::SystemManager* systemManager;// Or each scene stores its own system manager??
	cursed_engine::EntityFactory* entityFactory;
	cursed_engine::EventBus* eventBus;

	cursed_engine::ComponentRegistry* componentRegistry;

	//cursed_engine::TypeRegistry<cursed_engine::ComponentInfo, uint32_t>* componentData; //  todo, accept this in constructor...
	//Todo, store meta registry (component data)
};

class SceneLoader;

class Scene
{
public:
	// TODO; fix this... accept a scene contxt? - make sure to copy move (contains ECSRegistry)
	Scene(cursed_engine::SystemManager* systemManager, cursed_engine::EntityFactory* entityFactory, cursed_engine::ComponentRegistry* componentData, cursed_engine::EventBus* eventBus, std::string id);
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