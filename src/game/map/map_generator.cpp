#include "game/map/map_generator.h"
#include "game/map/map_types.h"
#include "game/map/tile_map.h"

//TileMap MapGenerator::generateMap(int seed) const
//{
//	return TileMap();
//}

void MapGenerator::generateStartArea(TileMap& map, int seed)
{
	MapChunk mapChunk;
	mapChunk.tileIds = std::vector<TileId>{
		1, 1, 1,
		1, 1, 1,
		1, 1, 1
	};

	map.insertMapChunk(std::move(mapChunk));
}

MapChunk MapGenerator::generateMapChunk(int x, int y) const
{
	return MapChunk();
}
