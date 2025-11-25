#include "engine/assets/asset_manager.h"

namespace cursed_engine
{
	void AssetManager::unloadAll()
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		m_cachesByType.clear();
		m_pathToHandles.clear();
	}
}