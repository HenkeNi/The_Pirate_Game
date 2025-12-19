#pragma once
#include "engine/utils/id_generator.h"
#include "engine/utils/type_traits.h"
#include <bitset>

namespace cursed_engine
{
	constexpr std::size_t MAX_SYSTEMS = 64;
	constexpr std::size_t MAX_ENTITIES = 100000; // 5000;
	constexpr std::uint8_t MAX_COMPONENTS = 64;
	constexpr std::uint32_t INVALID_ENTITY_ID = std::numeric_limits<std::uint32_t>::max();

	using ComponentID = std::uint8_t;
	using SystemID = std::uint8_t;
	using EntityID = uint32_t; // Or move to entity.h?
	using EntityVersion = uint32_t;
	using EntitySignature = std::bitset<MAX_COMPONENTS>;


	struct ComponentTag final {};
	struct SystemTag final {};

	template <ComponentType T>
	[[nodiscard]] ComponentID getComponentID() noexcept
	{
		return static_cast<ComponentID>(IDGenerator<ComponentTag>::GetID<T>());
	}

	class System;
	template <DerivedFrom<System> T>
	[[nodiscard]] SystemID getSystemID() noexcept
	{
		return static_cast<SystemID>(IDGenerator<SystemTag>::GetID<T>());
	}
}