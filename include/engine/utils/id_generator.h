#pragma once
#include <cstddef>

namespace cursed_engine
{
	template <typename Tag>
	class IdGenerator
	{
	public:
		template <typename T>
		[[nodiscard]] static std::size_t getId() noexcept;

	private:
		inline static std::size_t s_counter = 0;
	};

	template <typename Tag>
	template <typename T>
	[[nodiscard]] std::size_t IdGenerator<Tag>::getId() noexcept
	{
		static const std::size_t id = s_counter++;
		return id;
	}
}