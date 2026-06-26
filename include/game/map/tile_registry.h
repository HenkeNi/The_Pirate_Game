#pragma once
#include <engine/rendering/render_types.h>
#include <filesystem>
#include <vector>
#include <unordered_map>
#include <string>

using TileId = uint32_t; // here or in map

struct TileDefinition
{
    uint32_t spriteIndex;
    uint32_t layer;
    //TileID Id;
    //cursed_engine::UVRect uv;
    bool walkable;
    bool transparent;
};

struct TileSet
{
    // texture handle?
    std::unordered_map<uint32_t, TileDefinition> m_tiles;
    // png?
};

class TileRegistry
{
public:

    bool load(const std::filesystem::path& path);

    // read from json?

    //const TileDefinition& get(TileID id) const;

private:
    //std::vector<TileDefinition> m_tileDefinitions;
    std::unordered_map<std::string, TileSet> m_tileSets;
};
