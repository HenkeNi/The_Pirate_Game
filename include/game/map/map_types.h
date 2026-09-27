#pragma once
#include <engine/resources/resource_handle.h>
#include <engine/math/vec2.hpp>
#include <cstdint>
#include <string>
#include <unordered_map>

//#include <engine/rendering/render_types.h>
#include <engine/resources/texture/texture.h>

namespace map_constants
{
	constexpr int TILE_SIZE = 128; // unsigned leads to errors when calculating addition
	//constexpr uint32_t TILE_SIZE = 128;// 16; from atlas instead?
}

using TileId = uint32_t;
using TilesetId = std::string;

enum class TerrainType // remove?! -> read types from json...
{
	DeepWater,
	ShallowWater,
	Sand,
	Grass,
	Jungle
};


struct Spawnable
{
    std::string id; // ResourceId instead?
    float chance;
};

struct TileDefinition
{
    TileId id;
    std::vector<Spawnable> spawnables;

    uint32_t spriteIndex;
    uint32_t layer;
    cursed_engine::IVec2 atlasCoord; // or index?
    //cursed_engine::UVRect uv;
    bool walkable;
    bool transparent;
};

struct Tileset // just store textru handle and uvs?
{
    // or texture atlas handle?
    cursed_engine::ResourceHandle<cursed_engine::Texture> textureHandle; // sprite atlas handle?
    std::unordered_map<uint32_t, TileDefinition> tileTypes;
    //cursed_engine::IVec2 textureSize; // REMOVE?

   // std::string name; // redundant?
};