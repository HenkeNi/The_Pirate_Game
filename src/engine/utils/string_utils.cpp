#include "engine/utils/string_utils.h"
#include <algorithm>
#include <cctype>

namespace cursed_engine::helpers
{
	std::string toLowercase(std::string str)
	{
		std::transform(str.begin(), str.end(), str.begin(),
			[](unsigned char c) { return std::tolower(c); });
	
		return str;
	}

	std::string remove(std::string str, const std::string& substr)
	{
		std::size_t pos = str.find(substr);
		if (pos != std::string::npos)
		{
			str.erase(pos, str.length());
		}

		return str;
	}

	std::string removeLast(std::string str, const std::string& substr)
	{
		std::size_t pos = str.rfind(substr);
		if (pos != std::string::npos)
		{
			str.erase(pos, substr.length());
		}

		return str;
	}

	std::string removeSuffix(std::string str, const std::string& suffix)
	{
		std::size_t pos = str.find(suffix);


		return std::string();
	}
}