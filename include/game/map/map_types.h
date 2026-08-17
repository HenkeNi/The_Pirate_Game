#pragma once
#include <cstdint>
#include <string>

namespace map_constants
{
	constexpr int TILE_SIZE = 128; // unsigned leads to errors when calculating addition
	//constexpr uint32_t TILE_SIZE = 128;// 16; from atlas instead?
}

using TileId = uint32_t;
using TileSetId = std::string;

enum class TerrainType
{
	DeepWater,
	ShallowWater,
	Sand,
	Grass,
	Jungle
};