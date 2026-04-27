#pragma once
#include "engine/assets/asset_types.h"
#include "engine/resources/engine_resources.h"
#include <filesystem>
#include <optional>

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