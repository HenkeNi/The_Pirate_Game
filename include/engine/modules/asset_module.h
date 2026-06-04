#pragma once
#include "engine/assets/asset_manager.h"
#include "engine/core/localization/localization.h"

namespace cursed_engine
{
	class AssetModule
	{
	public:
		bool init();
		void shutdown();
		
		[[nodiscard]] inline AssetManager& getAssetManager() noexcept { return m_assetManager; }
		[[nodiscard]] inline Localization& getLocalization() noexcept { return m_localization; }

	private:
		AssetManager m_assetManager;
		Localization m_localization;
		// FIlesystem I/O
	};
}