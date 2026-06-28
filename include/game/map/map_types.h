#pragma once
#include <cstdint>
#include <string>

namespace map_constants
{
	constexpr uint32_t TILE_SIZE = 128;// 16; from atlas instead?
}

using TileId = uint32_t;
using TileSetId = std::string;
