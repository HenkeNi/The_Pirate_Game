#pragma once
#include "entity_handle.h" // or forward declare?
#include <optional>

#include <engine/ecs/component/component_registry.h> // for component init context...

namespace cursed_engine
{
	//class EntityHandle;
	//class ComponentRegistry;
	class ECSRegistry;
	class AssetManager;

	// Reanem PrefabInstantiator (or something) instead?

	class EntityFactory // TODO; accept asset mangaer instead!
	{
	public:
		EntityFactory(ComponentRegistry& registry/*, ComponentInitContext context*/);
		~EntityFactory() = default;

		void init(AssetManager* assetManager);				// only allow engine?
		void setEcsRegistry(ECSRegistry* ecsRegistry); 

		// OR RETURN AN INVALID ENTIY HANDLE INSTEAD? FROM CREATE_FROM_PREFAB...
		// contain functions like createEnemy, etc? contain logic to determine which type? sets other data? random strength, etc?
		std::optional<EntityHandle> createFromPrefab(const std::string& prefabId, FVec2 pos); // TODO; return EntityBuilder instead??? createEntity.withComponent<Transform>(data).withComponent().withTag("Player"´).build();
		//std::optional<EntityHandle> instantiate(ECSRegistry& ecsRegistry, std::string_view prefab);

		EntityHandle create();

		// TEMP
		void setContext(ComponentInitContext context)
		{
			m_initContext = std::move(context);
		}

		void setPostInitContext(ComponentPostInitContext context)
		{
			m_postInitContext = std::move(context);
		}

	private:
		//friend class Engine;

		//void initialize(AssetManager* assetManager); // why both? maybe just use public one

		ComponentRegistry& m_componentRegistry;
		AssetManager* m_assetManager; // weak ptr?
		ECSRegistry* m_ecsRegistry;

		ComponentInitContext m_initContext; // Maybe find better way?
		ComponentPostInitContext m_postInitContext;
	};
}