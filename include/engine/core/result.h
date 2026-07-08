#pragma once
#include <string>

namespace cursed_engine
{
	struct Result
	{
		bool succeeded;
		std::string message;

		static constexpr Result success() noexcept
		{
			return Result { true, std::string{} };
		}

		static constexpr Result failure(std::string msg) noexcept
		{
			return Result{ false, std::move(msg) };
		}
	};
}