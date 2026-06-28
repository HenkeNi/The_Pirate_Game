#include "game/map/map_generator.h"
#include "game/map/map_types.h"
#include "game/map/tile_map.h"

//TileMap MapGenerator::generateMap(int seed) const
//{
//	return TileMap();
//}

void MapGenerator::generateStartArea(TileMap& map, int seed)
{
	// FIX THIS!

	MapChunk mapChunk;

	auto& groundLayer = mapChunk.layers[(std::size_t)LayerType::Ground];

	groundLayer.tileIds = std::array<TileId, TileLayer::tileCount>{
		1, 1, 1,
		1, 1, 1,
		1, 1, 1
	};

	groundLayer.tileSetId = "island_tileset";
	groundLayer.isActive = true;

	map.insertMapChunk(std::move(mapChunk));
}

MapChunk MapGenerator::generateMapChunk(int x, int y) const
{
	return MapChunk();
}
