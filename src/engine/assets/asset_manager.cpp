#include "engine/assets/asset_manager.h"

namespace cursed_engine
{
	void AssetManager::unloadAll()
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		m_cachesByType.clear();
		m_pathToHandles.clear();
		m_idsToPaths.clear();
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
}