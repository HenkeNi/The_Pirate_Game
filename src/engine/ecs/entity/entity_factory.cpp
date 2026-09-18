#include "engine/ecs/entity/entity_factory.h"
#include "engine/ecs/component/component_registry.h"
#include "engine/assets/asset_manager.h"

namespace cursed_engine
{
	/*EntityFactory::EntityFactory(ECSRegistry& ecsRegistry, PrefabRegistry& prefabRegistry)
		: m_ecsRegistry{ ecsRegistry }, m_prefabRegistry{ prefabRegistry }
	{
	}*/

	/*EntityFactory::EntityFactory(AssetManager* assetManager)
		: m_ecsRegistry{ nullptr }, m_assetManager{ assetManager }
	{
	}*/

	//EntityFactory::EntityFactory(AssetManager* assetManager)
	//	: m_ecsRegistry{ nullptr }, m_assetManager{ assetManager }
	//{
	//}

	EntityFactory::EntityFactory(ComponentRegistry& registry/*, ComponentInitContext context*/)
		: m_componentRegistry{ registry }, m_assetManager{ nullptr }, m_ecsRegistry{ nullptr }/*, m_initContext{ std::move(context) }*/
	{
	}

	void EntityFactory::init(AssetManager* assetManager)
	{
		m_assetManager = assetManager;
	}

	void EntityFactory::setEcsRegistry(ECSRegistry* ecsRegistry)
	{
		m_ecsRegistry = ecsRegistry;
	}

	std::optional<EntityHandle> EntityFactory::createFromPrefab(const std::string& prefabId)
	{
		assert(m_ecsRegistry && "ECSRegistry is not set!");

		const AssetHandle assetHandle = m_assetManager->getAssetHandle<Prefab>(prefabId);

		const Prefab& prefab = m_assetManager->getAsset<Prefab>(assetHandle);

		// TODO; check if has parent? (heirarchy)

		EntityHandle entityHandle = m_ecsRegistry->createEntity();

		if (!entityHandle.isValid())
		{
			int x = 20;
		}

		for (const auto& [name, properties] : prefab.components)
		{
			const ComponentInfo& info = m_componentRegistry.get(name.c_str());
			info.instantation(entityHandle, properties, m_initContext);
		}

		//const auto& prefab = m_prefabRegistry->get(prefabId);

		// TODO; construct entity...

		return entityHandle;
	}

	EntityHandle EntityFactory::create()
	{
		return m_ecsRegistry->createEntity();
	}

	//void EntityFactory::initialize(AssetManager* assetManager)
	//{
	//	m_assetManager = assetManager;
	//}
}