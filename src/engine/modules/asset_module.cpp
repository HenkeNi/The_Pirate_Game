#include "engine/modules/asset_module.h"
#include "engine/assets/asset_loader.h"
#include <format>

namespace cursed_engine
{
	bool AssetModule::init()
	{
		Logger::logInfo(std::format("{}[AssetModule] - Initialization started...", log_format::INDENT));

		// TODO; create an AssetRegistry? or handle loading elsewhere? in Assets.h?
		// TODO; do lazy loading later on!? -> maybe load core resources?

		m_localization.registerLanguage("english", "../assets/localization/en.json"); // Dont here? read start language from config...
		m_localization.setLanguage("english");

		m_assetManager.addLoader<AnimationLoader>();
		m_assetManager.addLoader<TextureAtlasLoader>();
		m_assetManager.addLoader<PrefabLoader>();

		m_assetManager.addSearchPath("../assets/");

		Logger::logInfo(std::format("{}[AssetModule] - Initialization successful!", log_format::INDENT));
		return true;
	}

	void AssetModule::shutdown()
	{
		m_assetManager.unloadAll();
	}

	void AssetModule::scanAssets()
	{
		m_assetManager.scanAssets();
	}
}