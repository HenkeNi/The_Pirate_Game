#include "game/map/map_generator.h"
#include "game/map/map_types.h"
#include "game/map/tile_map.h"

#include <engine/core/logger.h>
#include <format>

MapGenerator::MapGenerator()
{
	auto& terrainNoise = m_settings.terrainNoise;

	terrainNoise.setType(cursed_engine::NoiseType::OpenSimplex2);
	terrainNoise.setSeed(1); // Todo, random seed!
	terrainNoise.setFrequency(1.f);
}

void MapGenerator::generateStartArea(TileMap& map, int seed)
{
	MapChunk mapChunk;
	
	auto& groundLayer = mapChunk.layers[(std::size_t)LayerType::Ground];

	for (int i = 0; i < TileLayer::tileCount; ++i)
	{
		int x = i % TileLayer::width;
		int y = i / TileLayer::width;

		float value = m_settings.terrainNoise.sample((float)x, (float)y);
		
		if (value < -0.04f)
		{

		}


		groundLayer.tileIds[i] = 1;
	}

	//groundLayer.tileIds = std::array<TileId, TileLayer::tileCount>{
	//	1, 1, 1,
	//	1, 1, 1,
	//	1, 1, 1
	//};

	groundLayer.tileSetId = "island_tileset";
	groundLayer.isActive = true;

	map.insertMapChunk(std::move(mapChunk));

	// send event map chunk generated...
}

MapChunk MapGenerator::generateMapChunk(int x, int y) const
{
	return MapChunk();
}
