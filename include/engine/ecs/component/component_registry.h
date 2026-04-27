#pragma once
#include "engine/utils/type_traits.h"
#include "engine/ecs/entity/entity_handle.h"
#include <engine/assets/asset_types.h>

#include "engine/core/type_registry.hpp"
#include <functional>

namespace cursed_engine
{
	class JsonValue;

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
		Deserialization deserialize; // Rename?
	};


	//template <ComponentType T, Callable<EntityHandle&, const ComponentProperties&> PrefabInstantiation, Callable<EntityHandle&, const JsonValue&> Deserialize> // better name than initfunc? use from componentinfo instead?
	//void registerComponent(ComponentRegistry& registry, const std::string& name, PrefabInstantiation&& instantation, Deserialize&& deserialization)
	//{
	//	const ComponentID id = getComponentID<T>();

	//	registry.emplace<T>(
	//		name,
	//		id,
	//		id,
	//		name,
	//		alignof(T),
	//		sizeof(T),
	//		std::forward<PrefabInstantiation>(instantation),
	//		std::forward<Deserialize>(deserialization)
	//	);
	//}


	class JsonValue;

	class ComponentRegistry
	{
	public:
		template <ComponentType T, Callable<EntityHandle&, const ComponentProperties&> PrefabInstantiation, Callable<EntityHandle&, const JsonValue&> Deserialize> // better name than initfunc? use from componentinfo instead?
		void registerComponent(const std::string& name, PrefabInstantiation&& instantation, Deserialize&& deserialization)
		{
			const ComponentID id = getComponentID<T>();

			m_registry.emplace<T>(
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

		inline const ComponentInfo& get(const char* name) const
		{
			return m_registry.get(name);
		}

		const ComponentInfo* tryGet(const char* name) const
		{
			return m_registry.tryGet(name);
		}


		inline bool isValid(uint32_t id) const
		{
			return m_registry.isValid(id);
		}

		inline bool isValid(const std::string& name) const
		{
			return m_registry.isValid(name);
		}


	private:
		TypeRegistry<struct ComponentInfo, uint32_t> m_registry;
	};
}