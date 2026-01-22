#include "engine/ecs/entity/entity_factory.h"
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

	void EntityFactory::init(AssetManager* assetManager)
	{
		m_assetManager = assetManager;
	}

	void EntityFactory::setEcsRegistry(ECSRegistry* ecsRegistry)
	{
		m_ecsRegistry = ecsRegistry;
	}

	std::optional<EntityHandle> EntityFactory::createFromPrefab(const std::string& prefabID)
	{
		//const auto& prefab = m_prefabRegistry->get(prefabID);

		// TODO; construct entity...

		return std::nullopt;
	}

	void EntityFactory::initialize(AssetManager* assetManager)
	{
		m_assetManager = assetManager;
	}
}