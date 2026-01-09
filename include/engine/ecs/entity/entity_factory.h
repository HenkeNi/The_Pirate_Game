#pragma once
#include "entity_handle.h" // or forward declare?
#include <optional>

namespace cursed_engine
{
	//class EntityHandle;
	class ECSRegistry;
	class AssetManager;

	// Reanem PrefabInstantiator (or something) instead?

	class EntityFactory // TODO; accept asset mangaer instead!
	{
	public:
		EntityFactory(AssetManager* assetManager = nullptr);

		void setEcsRegistry(ECSRegistry* ecsRegistry);

		// contain functions like createEnemy, etc? contain logic to determine which type? sets other data? random strength, etc?
		std::optional<EntityHandle> createFromPrefab(const std::string& prefabID);
		//std::optional<EntityHandle> instantiate(ECSRegistry& ecsRegistry, std::string_view prefab);


	private:
		friend class Engine;

		void initialize(AssetManager* assetManager);

		ECSRegistry* m_ecsRegistry;
		AssetManager* m_assetManager;
	};
}