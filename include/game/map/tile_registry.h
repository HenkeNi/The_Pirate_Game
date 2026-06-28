#pragma once
#include "game/map/map_types.h"
#include <engine/rendering/render_types.h>
#include <engine/resources/resource_handle.h>
#include <filesystem>
#include <vector>
#include <unordered_map>
#include <string>

namespace cursed_engine
{
    class Texture;
    class AssetManager;
}

struct TileDefinition
{
    TileId id;
    
    uint32_t spriteIndex;
    uint32_t layer;
    cursed_engine::IVec2 atlasCoord; // or index?
    //cursed_engine::UVRect uv;
    bool walkable;
    bool transparent;
};

struct TileSet // just store textru handle and uvs?
{
    // or texture atlas handle?
    cursed_engine::ResourceHandle<cursed_engine::Texture> textureHandle; // sprite atlas handle?
    std::unordered_map<uint32_t, TileDefinition> tileTypes;
    cursed_engine::IVec2 textureSize;
};

class TileRegistry
{
public:

    bool load(cursed_engine::AssetManager& assetManager, const std::filesystem::path& path);

    // read from json?

    // getTileSet?

    [[nodiscard]] const TileSet* getTileSet(const TileSetId& id) const noexcept;
   // [[nodiscard]] const TileDefinition& get(TileId id) const;

private:
    //std::vector<TileDefinition> m_tileDefinitions;
    std::unordered_map<TileSetId, TileSet> m_tileSets; // or store in vector, use map to map string to index?
};
