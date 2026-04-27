#pragma once
#include <string>

namespace cursed_engine::helpers
{
	// constexpr? and noexcept?
	[[nodiscard]] constexpr std::string toLowercase(std::string str);

	[[nodiscard]] std::string remove(std::string str, const std::string& substr);

	[[nodiscard]] std::string removeLast(std::string str, const std::string& substr);

	[[nodiscard]] std::string removeSuffix(std::string str, const std::string& suffix);
	}