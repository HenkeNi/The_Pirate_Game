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
	struct SpawnData;
	
	class EntityFactory // TODO; accept asset mangaer instead!
	{
	public:
		EntityFactory(ComponentRegistry& registry/*, ComponentInitContext context*/);
		~EntityFactory() = default;

		void init(AssetManager* assetManager);				// only allow engine?
		void setEcsRegistry(ECSRegistry* ecsRegistry); 

		// OR RETURN AN INVALID ENTIY HANDLE INSTEAD? FROM CREATE_FROM_PREFAB...
		// contain functions like createEnemy, etc? contain logic to determine which type? sets other data? random strength, etc?
		EntityHandle createFromPrefab(const std::string& prefabId, const SpawnData& data); 

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
		ComponentRegistry& m_componentRegistry;
		AssetManager* m_assetManager; // weak ptr?
		ECSRegistry* m_ecsRegistry;

		ComponentInitContext m_initContext; // Maybe find better way?
		ComponentPostInitContext m_postInitContext;
	};
}