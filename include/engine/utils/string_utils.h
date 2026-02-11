#pragma once
#include <string>

namespace cursed_engine::helpers
{
	std::string toLowercase(std::string str);

	std::string remove(std::string str, const std::string& substr);

	std::string removeLast(std::string str, const std::string& substr);

	std::string removeSuffix(std::string str, const std::string& suffix);
}