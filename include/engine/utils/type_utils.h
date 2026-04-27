#pragma once
#include <typeindex>

namespace cursed_engine
{
	// TODO; mark constexpr?

	template <typename T>
	[[nodiscard]] inline std::type_index getTypeIndex() noexcept
	{
		return std::type_index(typeid(std::remove_cvref_t<T>));
	}
}