#pragma once
#include "engine/utils/type_traits.h"
#include "engine/ecs/entity/entity_handle.h"
#include <engine/assets/asset_types.h>
#include "engine/core/registry_aliases.h"

namespace cursed_engine
{
	// rename file type_infos.hpp?
	// Include EnityHandle? include componet propoerties?

	//struct ComponentInfo;
	class JsonValue;

	// TODO; or dont have separate functions for init and add?

	// name init func something with prefab? PrefabInstantiation?
	template <ComponentType T, Callable<EntityHandle&, const ComponentProperties&> PrefabInstantiation, Callable<EntityHandle&, const JsonValue&> Deserialize> // better name than initfunc? use from componentinfo instead?
	void registerComponent(ComponentRegistry& registry, const std::string& name, PrefabInstantiation&& instantation, Deserialize&& deserialization)
	{
		const ComponentID id = getComponentID<T>();

		registry.emplace<T>(
			name, 
			id, 
			id, 
			name, 
			alignof(T), 
			sizeof(T), 
			std::forward<PrefabInstantiation>(instantation),
			std::forward<Deserialize>(deserialization)
		);		
	}

	struct ComponentInfo
	{
		ComponentID id;
		std::string name;
		std::size_t alignment;
		std::size_t size;

		// TODO; find a better name
		using Deserialization = std::function<void(EntityHandle& handle, const JsonValue& value)>; 
		using PrefabInstantiation = std::function<void(EntityHandle& handle, const ComponentProperties& properties)>; // prefabInstance

		PrefabInstantiation instantation;
		Deserialization deserialize;
	};

	struct SystemInfo
	{
	};

	// Put in game!
	//struct SceneInfo
	//{
	//};

}