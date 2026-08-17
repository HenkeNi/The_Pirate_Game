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

		for (const auto& entry : std::filesystem::recursive_directory_iterator("../assets/"))
		{
			if (!entry.is_regular_file())
				continue;

			const auto& path = entry.path();
			std::string filename = path.string();

			if (filename.ends_with("animation.json"))
			{
				auto handle = m_assetManager.loadAsset<AnimationSet>(path);
			}
			else if (filename.ends_with("texture_atlas.json"))
			{
				auto handle = m_assetManager.loadAsset<TextureAtlas>(path);
			}
			else if (filename.ends_with("prefab.json"))
			{
				auto handle = m_assetManager.loadAsset<Prefab>(entry.path());
			}
		}

		Logger::logInfo(std::format("{}[AssetModule] - Initialization successful!", log_format::INDENT));
		return true;
	}

	void AssetModule::shutdown()
	{

	}

	//AssetServices AssetModule::getServices() noexcept
	//{
	//	return { &m_assetManager, &m_localization };
	//}
}