#pragma once
#include "engine/assets/asset_types.h"
#include <filesystem>
#include <optional>

// [Consider] - instead of returning optionals, allow assets to be invalid
// [Consider] - make loaders struct? don't use inheritance?

namespace cursed_engine
{
	class AssetLoaderBase
	{
	public:
		virtual ~AssetLoaderBase() = default;
	};

	template <typename T>
	class AssetLoader : public AssetLoaderBase
	{
	public:
		using AssetType = T;

		[[nodiscard]] virtual std::optional<T> load(const std::filesystem::path& path) const = 0;
	};


	class TextureAtlasLoader : public AssetLoader<TextureAtlas>
	{
	public:
		[[nodiscard]] std::optional<TextureAtlas> load(const std::filesystem::path& path) const override;
	};


	class SpriteSheetLoader : public AssetLoader<SpriteSheet>
	{
	public:
		[[nodiscard]] std::optional<SpriteSheet> load(const std::filesystem::path& path) const override;
	};


	class PrefabLoader : public AssetLoader<Prefab>
	{
	public:
		[[nodiscard]] std::optional<Prefab> load(const std::filesystem::path& path) const override;
	};
}