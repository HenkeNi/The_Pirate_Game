#include "engine/assets/asset_manager.h"

namespace cursed_engine
{
	void AssetManager::scanAssets(std::filesystem::path path)
	{
		for (const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(path))
		{
			if (!entry.is_regular_file())
				continue;

			const std::filesystem::path& path = entry.path();
			const std::string filename = path.string();

			auto it = std::find_if(m_assetTypes.begin(), m_assetTypes.end(),
				[&](const auto& pair)
				{
					//pair.second.loader->format();
					return filename.ends_with(pair.second.loader->format());
				});

			if (it != m_assetTypes.end())
			{
				AssetType& assetType = it->second;
				std::string id = extractAssetId(path, assetType.loader->format());

				assetType.idToMetaData.insert_or_assign(std::move(id), AssetMetaData{ path });

				//m_idsToPaths.insert_or_assign(std::move(id), path);
			}
		}
	}

	void AssetManager::unloadAll()
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		for (auto& assetType : m_assetTypes)
		{
			assetType.second.cache = nullptr;
			assetType.second.loader = nullptr;

			assetType.second.idToAssetHandle.clear();
			assetType.second.idToMetaData.clear();
		}

		//m_cachesByType.clear();
		//m_loadersByType.clear();
		//m_idToAssetHandle.clear();
		//m_idToMetaData.clear();
		//m_pathToHandles.clear();
		//m_idsToPaths.clear();
	}

	//std::string AssetManager::extractIdentifier(const std::filesystem::path& path)
	//{
	//	std::string filename = path.filename().string();

	//	// TODO; put in utility file? FileUtils? static member?
	//	static const std::vector<std::string> extensions = {
	//		".texture_atlas.json",
	//		".material.json",
	//		".animation.json",
	//		".sprite_sheet.json"
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

	std::string AssetManager::extractAssetId(const std::filesystem::path& path, const std::string& format) const
	{
		std::string filename = path.filename().string();

		return filename.substr(0, filename.length() - format.length());


		// TODO; put in utility file? FileUtils? static member?
		/*static const std::vector<std::string> extensions = {
			".texture_atlas.json",
			".material.json",
			".animation.json",
			".sprite_sheet.json"
		};*/


		/*for (const auto& extension : extensions)
		{
			if (filename.ends_with(extension))
			{
				return filename.substr(0, filename.length() - extension.length());
			}
		}*/

		/*size_t firstDot = filename.find('.');
		if (firstDot != std::string::npos && firstDot > 0)
		{
			return filename.substr(0, firstDot);
		}

		return filename;*/
	}
}