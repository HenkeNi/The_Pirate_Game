#pragma once
#include <cstddef>

namespace cursed_engine
{
	template <typename Tag>
	class IDGenerator
	{
	public:
		template <typename T>
		[[nodiscard]] static std::size_t getID() noexcept;

	private:
		inline static std::size_t s_counter = 0;
	};

	template <typename Tag>
	template <typename T>
	[[nodiscard]] std::size_t IDGenerator<Tag>::getID() noexcept
	{
		static const std::size_t id = s_counter++;
		return id;
	}
}