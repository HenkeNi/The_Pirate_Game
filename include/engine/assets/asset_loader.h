#pragma once
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
}