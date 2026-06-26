#pragma once

namespace cursed_engine
{
	class EntityFactory;
	class ComponentRegistry;
	class SystemManager;
	class EventBus;
}

struct SceneContext
{
	cursed_engine::EntityFactory* entityFactory;
	cursed_engine::ComponentRegistry* componentRegistry;
	cursed_engine::SystemManager* systemManager;
	cursed_engine::EventBus* eventBus;
};