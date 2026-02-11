#include "engine/utils/path_utils.h"
#include <vector>

namespace cursed_engine
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

	std::string extractResourceID(const std::filesystem::path& path)
	{
		std::string filename = path.filename().string();

		// TODO; put in utility file? FileUtils? static member?
		static const std::vector<std::string> extensions = {
			".jpeg",
			".png",
			".bmp",
			".mp3",
			".wav",
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
}