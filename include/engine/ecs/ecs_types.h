#pragma once
#include "engine/utils/concepts.h"
#include "engine/utils/id_generator.h"
#include <bitset>

namespace cursed_engine
{
	// dont use std::?
	constexpr std::size_t MAX_SYSTEMS = 64;
	constexpr std::size_t MAX_ENTITIES = 100000; // 5000;
	constexpr std::uint8_t MAX_COMPONENTS = 64;
	constexpr std::uint32_t INVALID_ENTITY_ID = std::numeric_limits<std::uint32_t>::max();
	constexpr std::uint32_t INVALID_ENTITY_VERSION = 0;

	// TOOD; use Id not ID?
	using ComponentId = std::uint8_t; 
	using SystemId = std::uint8_t;

	using EntityId = uint32_t; // Or move to entity.h?
	using EntityVersion = uint32_t;
	using EntitySignature = std::bitset<MAX_COMPONENTS>;


	struct ComponentTag final {};
	//struct SystemTag final {};
	struct UpdateSystemTag final {};
	struct RenderSystemTag final {};

	template <ComponentType T>
	[[nodiscard]] ComponentId getComponentId() noexcept
	{
		return static_cast<ComponentId>(IdGenerator<ComponentTag>::getId<T>());
	}

	class UpdateSystem;

	template <DerivedFrom<UpdateSystem> T>
	[[nodiscard]] SystemId getUpdateSystemId() noexcept
	{
		return static_cast<SystemId>(IdGenerator<UpdateSystemTag>::getId<T>());
	}
	
	class RenderSystem;
	
	template <DerivedFrom<RenderSystem> T>
	[[nodiscard]] SystemId getRenderSystemId() noexcept
	{
		return static_cast<SystemId>(IdGenerator<RenderSystemTag>::getId<T>());
	}
}