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
}