#pragma once
#include "engine/utils/concepts.h"
#include "engine/ecs/entity/entity_handle.h"
#include "engine/assets/asset_types.h"

//#include "engine/rendering/render_api.h"
//#include "engine/rendering/render_types.h" // Needed??

#include "engine/resources/resource_types.h"

#include "engine/utils/containers/registry.hpp"
#include <functional>
#include <string>

#include "engine/rendering/render_api.h"

// TODO; rename ecs_types.h?

namespace cursed_engine
{
	class JsonValue;

	struct ComponentInitContext
	{
		class AssetManager* assetManager{};
		class Localization* localization{};
		//RenderAPI renderer{}; // WHY???????????????????

		AudioManager* audioManager{};
		FontManager* fontManager{};
		TextureManager* textureManager{};
		class TextManager* textManager{};
		//class TextFactory* textFactory{};
	};

	template <typename Context>
	ComponentInitContext createComponentInitContext(const Context& context)
	{
		return ComponentInitContext{
			context.assets.assetManager,
			context.assets.localization,
			//context.rendering.rendererAPI,
			context.resources.audioManager,
			context.resources.fontManager,
			context.resources.textureManager,
			context.resources.textManager,
		};
	}

	struct ComponentInfo
	{
		ComponentId id;
		std::string name;
		std::size_t alignment;
		std::size_t size;

		// TODO; find a better name
		using Deserialization = std::function<void(EntityHandle& handle, const JsonValue& value, const ComponentInitContext& context)>;
		using PrefabInstantiation = std::function<void(EntityHandle& handle, const ComponentProperties& properties, const ComponentInitContext& ctx)>; // prefabInstance

		PrefabInstantiation instantation;
		Deserialization deserialize; // Rename?
	};

	using ComponentName = std::string;

	using ComponentRegistry = Registry<ComponentName, struct ComponentInfo>;
	//using ComponentRegistry = Registry<struct ComponentInfo, ComponentId>;

	template <ComponentType T, typename... Args>
	void registerComponent(ComponentRegistry& registry, std::string name, Args&&... args)
	{
		const ComponentId id = getComponentId<T>();

		registry.emplace(
			name,
			id,
			name,
			alignof(T),
			sizeof(T),
			std::forward<Args>(args)...
		);
	}

	//template <ComponentType T, Callable<EntityHandle&, const ComponentProperties&> PrefabInstantiation, Callable<EntityHandle&, const JsonValue&> Deserialize> // better name than initfunc? use from componentinfo instead?
	//void registerComponent(ComponentRegistry& registry, const std::string& name, PrefabInstantiation&& instantation, Deserialize&& deserialization)
	//{
	//	const ComponentID id = getComponentID<T>();

	//	registry.emplace(
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


	
	// insert into asset manager????
	//class ComponentRegistry
	//{
	//public:
	//	template <ComponentType T, typename... Args>
	//	void registerComponent(std::string name, Args&&... args)
	//	{
	//		const ComponentId id = getComponentId<T>();

	//		m_registry.emplace<T>(
	//			name,
	//			id,
	//			id,
	//			name,
	//			alignof(T),
	//			sizeof(T),
	//			std::forward<Args>(args)...
	//		);
	//	}

	//	//template <ComponentType T, Callable<EntityHandle&, const ComponentProperties&, const ComponentInitContext&> PrefabInstantiation, Callable<EntityHandle&, const JsonValue&, const ComponentInitContext&> Deserialize> // better name than initfunc? use from componentinfo instead?
	//	//void registerComponent(const std::string& name, PrefabInstantiation&& instantation, Deserialize&& deserialization)
	//	//{
	//	//	// TODO; static assert that name is lowercase?
	//	//	const ComponentId id = getComponentId<T>();

	//	//	m_registry.emplace<T>(
	//	//		name,
	//	//		id,
	//	//		id,
	//	//		name,
	//	//		alignof(T),
	//	//		sizeof(T),
	//	//		std::forward<PrefabInstantiation>(instantation),
	//	//		std::forward<Deserialize>(deserialization)
	//	//	);
	//	//}

	//	inline const ComponentInfo& get(const char* name) const
	//	{
	//		return m_registry.get(name);
	//	}

	//	const ComponentInfo* tryGet(const char* name) const
	//	{
	//		return m_registry.tryGet(name);
	//	}

	//	inline bool isValid(uint32_t id) const
	//	{
	//		return m_registry.isValid(id);
	//	}

	//	inline bool isValid(const std::string& name) const
	//	{
	//		return m_registry.isValid(name);
	//	}

	//	inline void clear()
	//	{
	//		m_registry.clear();
	//	}

	//private:
	//	Registry<struct ComponentInfo, ComponentId> m_registry;
	//};
}