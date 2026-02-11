#pragma once
#include "engine/assets/asset_loader.h"
#include "engine/assets/asset_types.h"
#include "engine/resources/engine_resources.h"

namespace cursed_engine
{
	class TextureAtlasLoader : public AssetLoader<TextureAtlas>
	{
	public:
		TextureAtlasLoader(EngineResources& resources);
		[[nodiscard]] std::optional<TextureAtlas> load(const std::filesystem::path& path) const override;

	private:
		EngineResources& m_resources;
	};


	class SpriteSheetLoader : public AssetLoader<SpriteSheet>
	{
	public:
		SpriteSheetLoader(EngineResources& resources);
		[[nodiscard]] std::optional<SpriteSheet> load(const std::filesystem::path& path) const override;
	
	private:
		EngineResources& m_resources;
	};


	class PrefabLoader : public AssetLoader<Prefab>
	{
	public:
		[[nodiscard]] std::optional<Prefab> load(const std::filesystem::path& path) const override;
	};
}