#pragma once 
#include "game/map/map_types.h"
#include <engine/assets/asset_loader.h>

namespace cursed_engine
{
	class AssetManager;

	template <typename T>
	class Result;
}

namespace ce = cursed_engine;

// TODO; add value to result class? return REsult instead in load function...

class TilesetLoader : public ce::AssetLoader<Tileset>
{
public:
	TilesetLoader(cursed_engine::AssetManager& assetManager);

	[[nodiscard]] ce::Result<Tileset> load(const std::filesystem::path& path) const override;
	[[nodiscard]] const char* format() const override;

private:
	cursed_engine::AssetManager& m_assetManager;
};