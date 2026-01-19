#pragma once
#include "engine/utils/type_traits.h"
#include "engine/ecs/entity/entity_handle.h"

namespace cursed_engine
{
	// rename file type_infos.hpp?
	// Include EnityHandle? include componet propoerties?

	struct ComponentInfo;

	template <ComponentType T, Callable<EntityHandle&, const ComponentProperties&> InitFunc> // better name than initfunc? use from componentinfo instead?
	void registerComponent(TypeRegistry<ComponentInfo>& registry, const std::string& name, InitFunc&& func)
	{
		const ComponentID id = getComponentID<T>();

		registry.emplace<T>(name, id, id, name, alignof(T), sizeof(T), std::forward<InitFunc>(func),
			[](EntityHandle& handle)
			{
				handle.attachComponent<T>();
			});
	}

	struct ComponentInfo
	{
		using InitFunc = std::function<void(EntityHandle& handle, const ComponentProperties& properties)>;
		using AddComponentFunc = std::function<void(EntityHandle& handle)>;

		ComponentID id;
		std::string name;
		std::size_t alignment;
		std::size_t size;

		// TODO; need to include ComponentProperties? or forward declare it...
		InitFunc initialization;
		AddComponentFunc addComponent;
	};

	struct SystemInfo
	{
	};

	// Put in game!
	//struct SceneInfo
	//{
	//};

}