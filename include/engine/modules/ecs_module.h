#pragma once
#include "engine/ecs/component/component_registry.h"
#include "engine/ecs/entity/entity_factory.h"
#include "engine/ecs/system/system_manager.h"

namespace cursed_engine
{
	struct EngineContext;

	class ECSModule
	{
	public:
		bool init(const EngineContext& context);
		void shutdown();

		[[nodiscard]] inline SystemManager& getSystemManager() noexcept { return m_systemManager; }
		[[nodiscard]] inline EntityFactory& getEntityFactory() noexcept { return m_entityFactory; }
		[[nodiscard]] inline ComponentRegistry& getComponentRegistry() noexcept { return m_componentRegistry; }
		
	private:
		void registerCoreComponents();

		SystemManager m_systemManager;
		EntityFactory m_entityFactory;
		ComponentRegistry m_componentRegistry;
	};
}