#include "engine/core/file_catalog.h"

namespace
{
	std::string extractResourceId(const std::filesystem::path& path) noexcept
	{
		return path.stem().string();
		/*std::string filename = path.filename().string();

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

		return filename;*/
	}
}

namespace cursed_engine
{
	void FileCatalog::scanDirectory(const std::filesystem::path& root)
	{
		//for (const auto& entry : std::filesystem::recursive_directory_iterator("../assets"))
		for (const auto& entry : std::filesystem::recursive_directory_iterator("../assets"))
		{
			if (!entry.is_regular_file())
				continue;

			const auto& path = entry.path();

			std::string filename = path.string();

			//if (hasValidFileExtension(filename, resourceConfig.validTextureFormats)
			//	|| hasValidFileExtension(filename, resourceConfig.validAudioFormats)
			//	|| hasValidFileExtension(filename, resourceConfig.validFontFormats))
			//{
			//	auto id = extractResourceId(path);
			//	resourceConfig.resourceIdToPath.insert_or_assign(std::move(id), path.string());
			//}

			auto id = extractResourceId(path);
			//resourceConfig.resourceIdToPath.insert_or_assign(std::move(id), path.string());

		}
	}

	bool hasValidFileExtension(const std::string& filename, const std::vector<std::string>& extensions) noexcept
	{
		for (const auto& extension : extensions)
		{
			if (filename.ends_with(extension))
				return true;
		}

		return false;
	}

}