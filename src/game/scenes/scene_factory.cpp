#include "game/scenes/scene_factory.h"
#include "game/scenes/scene.h"
#include "game/scenes/title_scene.h"
#include "game/scenes/settings_scene.h"
#include <engine/core/application.h>

//void SceneFactory::init(cursed_engine::SystemManager* systemManager, cursed_engine::EntityFactory* entityFactory, cursed_engine::EventBus* eventBus, cursed_engine::ComponentRegistry* componentRegistry)
//{
//	m_systemManager = systemManager; 
//	m_entityFactory = entityFactory; 
//	m_eventBus = eventBus;
//	m_componentRegistry = componentRegistry;
//}

void SceneFactory::init(cursed_engine::ComponentInitContext context, cursed_engine::SystemManager* systemManager, cursed_engine::EntityFactory* entityFactory, cursed_engine::ComponentRegistry* componentRegistry, cursed_engine::EventBus* eventBus)
{
	m_context = context;
	m_systemManager = systemManager;
	m_entityFactory = entityFactory;
	m_componentRegistry = componentRegistry;

	m_eventBus = eventBus;
}

std::unique_ptr<Scene> SceneFactory::createScene(const std::string& scene)
{
	if (scene == "title_scene")
	{
		return std::make_unique<TitleScene>(m_systemManager, m_entityFactory, m_componentRegistry, m_eventBus);
	}
	else if (scene == "settings_scene")
	{
		return std::make_unique<SettingsScene>(m_systemManager, m_entityFactory, m_componentRegistry, m_eventBus);
	}
	return nullptr;
}
