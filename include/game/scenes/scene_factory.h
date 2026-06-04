#pragma once
#include <engine/ecs/component/component_registry.h>
#include <memory>
#include <string>

namespace cursed_engine
{
	//struct ComponentInitContext;

	class SystemManager;
	class EntityFactory;
	class EventBus;
	//class ComponentRegistry;
}

class Scene;
//struct SceneData;

class SceneFactory
{
public:

	// Function for creating contexts???? 

	// LOOK OVER THESE PARAMS!!!!!!!!!!!!!
	// accept context instead??
	void init(cursed_engine::ComponentInitContext context, cursed_engine::SystemManager* systemManager, cursed_engine::EntityFactory* entityFactory, cursed_engine::ComponentRegistry* componentRegistry, cursed_engine::EventBus* eventBus);
	//void init(cursed_engine::ComponentInitContext context, cursed_engine::ECSServices services, cursed_engine::EventBus* eventBus);
	std::unique_ptr<Scene> createScene(const std::string& scene);
	//std::unique_ptr<Scene> createScene(const char* scene, const SceneData& data);

private:
	cursed_engine::ComponentInitContext m_context;

	//cursed_engine::ECSServices m_services;
	cursed_engine::EventBus* m_eventBus;
	cursed_engine::SystemManager* m_systemManager;// Or each scene stores its own system manager??
	cursed_engine::EntityFactory* m_entityFactory;
	cursed_engine::ComponentRegistry* m_componentRegistry;
};