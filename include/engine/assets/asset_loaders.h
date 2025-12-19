#pragma once
#include "engine/assets/asset_loader.h"
#include "engine/assets/asset_types.h"
#include "engine/resources/engine_resources.h"

namespace cursed_engine
{
	struct TextureAtlas;

	class TextureAtlasLoader : public AssetLoader<TextureAtlas>
	{
	public:
		TextureAtlasLoader(EngineResources& resources);
		std::optional<TextureAtlas> load(const std::string& path) const override;

	private:
		EngineResources& m_resources;
	};

	struct SpriteSheet;

	class SpriteSheetLoader : public AssetLoader<SpriteSheet>
	{
	public:
		SpriteSheetLoader(EngineResources& resources);
		std::optional<SpriteSheet> load(const std::string& path) const override;
	
	private:
		EngineResources& m_resources;
	};


	struct Prefab;

	class PrefabLoader : public AssetLoader<Prefab>
	{
	public:
		std::optional<Prefab> load(const std::string& path) const override;

		//[[nodiscard]] std::optional<std::pair<std::string, Prefab>> load(const std::filesystem::path& path) const;
	};


}