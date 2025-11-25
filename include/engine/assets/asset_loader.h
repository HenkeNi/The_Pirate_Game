#pragma once
#include <string>
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

		[[nodiscard]] virtual std::optional<T> load(const std::string& path) const = 0; // TODO; nodiscard? safe to mark const? should load modify?
	};
}