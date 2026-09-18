#pragma once
#include "game/map/map_types.h"
#include <engine/rendering/render_types.h>
#include <engine/resources/resource_handle.h>
#include <filesystem>
#include <vector>
#include <unordered_map>
#include <string>

//namespace cursed_engine
//{
//    class Texture;
//    class AssetManager;
//}
//
//
//// Rename TileTypeRegistry?
//class TileRegistry
//{
//public:
//
//    //bool load(cursed_engine::AssetManager& assetManager, const std::filesystem::path& path);
//
//    // read from json?
//
//    // getTileSet?
//
//   // [[nodiscard]] const Tileset* getTileset(const TilesetId& id) const noexcept;
//   // [[nodiscard]] const TileDefinition& get(TileId id) const;
//
//private:
//    //std::vector<TileDefinition> m_tileDefinitions;
//    //std::unordered_map<TilesetId, Tileset> m_tilesets; // or store in vector, use map to map string to index?
//};
