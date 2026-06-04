#include "engine/utils/utils.h"
#include <algorithm>
#include <cctype>
#include <vector>

namespace cursed_engine::utils
{
	std::string extractAssetID(const std::filesystem::path& path)
	{
		std::string filename = path.filename().string();

		// TODO; put in utility file? FileUtils? static member?
		static const std::vector<std::string> extensions = {
			".texture_atlas.json",
			".material.json",
			".animation.json",
			".sprite_sheet.json"
		};

		for (const auto& extension : extensions)
		{
			if (filename.ends_with(extension))
			{
				return filename.substr(0, filename.length() - extension.length());
			}
		}

		size_t firstDot = filename.find('.');
		if (firstDot != std::string::npos && firstDot > 0)
		{
			return filename.substr(0, firstDot);
		}

		return filename;
	}

	//std::string extractResourceID(const std::filesystem::path& path)
	//{
	//	std::string filename = path.filename().string();

	//	// TODO; put in utility file? FileUtils? static member?
	//	static const std::vector<std::string> extensions = {
	//		".jpeg",
	//		".png",
	//		".bmp",
	//		".mp3",
	//		".wav",
	//	};

	//	for (const auto& extension : extensions)
	//	{
	//		if (filename.ends_with(extension))
	//		{
	//			return filename.substr(0, filename.length() - extension.length());
	//		}
	//	}

	//	size_t firstDot = filename.find('.');
	//	if (firstDot != std::string::npos && firstDot > 0)
	//	{
	//		return filename.substr(0, firstDot);
	//	}

	//	return filename;
	//}

	namespace string_helpers
	{
		constexpr std::string toLowercase(std::string str)
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
}