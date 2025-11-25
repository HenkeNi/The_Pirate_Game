#pragma once
#include "engine/assets/asset_loader.h"
#include "engine/assets/asset_types.h"

namespace cursed_engine
{
	struct TextureAtlas;

	class TextureAtlasLoader : public AssetLoader<TextureAtlas>
	{
	public:
		std::optional<TextureAtlas> load(const std::string& path) const override;
	};


	struct SpriteSheet;

	class SpriteSheetLoader : public AssetLoader<SpriteSheet>
	{
	public:
		std::optional<SpriteSheet> load(const std::string& path) const override;
	};


	struct Prefab;

	class PrefabLoader : public AssetLoader<Prefab>
	{
	public:
		std::optional<Prefab> load(const std::string& path) const override;

		//[[nodiscard]] std::optional<std::pair<std::string, Prefab>> load(const std::filesystem::path& path) const;
	};


}