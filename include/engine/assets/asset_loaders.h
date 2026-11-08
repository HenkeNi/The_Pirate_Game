#pragma once
#include "engine/assets/asset_types.h"
#include <filesystem>

// [Consider] - instead of returning optionals, allow assets to be invalid
// [Consider] - make loaders struct? don't use inheritance?

namespace cursed_engine
{
	template <typename T>
	class Result;

	class AssetLoaderBase
	{
	public:
		virtual ~AssetLoaderBase() = default;

		[[nodiscard]] virtual const char* format() const = 0;
	};

	template <typename T>
	class AssetLoader : public AssetLoaderBase
	{
	public:
		using AssetType = T;

		[[nodiscard]] virtual Result<T> load(const std::filesystem::path& path) const = 0; 
	};


	class TextureAtlasLoader : public AssetLoader<TextureAtlas>
	{
	public:
		[[nodiscard]] Result<TextureAtlas> load(const std::filesystem::path& path) const override;
		[[nodiscard]] const char* format() const override;
	};

	// rename animation loader...
	class AnimationLoader : public AssetLoader<AnimationSet>
	{
	public:
		[[nodiscard]] Result<AnimationSet> load(const std::filesystem::path& path) const override;
		[[nodiscard]] const char* format() const override;
	};

	class PrefabLoader : public AssetLoader<Prefab>
	{
	public:
		[[nodiscard]] Result<Prefab> load(const std::filesystem::path& path) const override;
		[[nodiscard]] const char* format() const override;
	};
}