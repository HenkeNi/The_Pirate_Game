#pragma once

namespace cursed_engine
{
	// TODO; or put in Types.h?

	template <typename T, typename ID>
	class TypeRegistry;

	using ComponentRegistry = TypeRegistry<struct ComponentInfo, uint32_t>; // Rename ComponentMeta?
}