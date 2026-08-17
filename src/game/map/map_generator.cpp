#include "game/map/map_generator.h"
#include "game/map/map_types.h"
#include "game/map/tile_map.h"

#include <engine/core/logger.h>
#include <format>

MapGenerator::MapGenerator()
{
	auto& terrainNoise = m_settings.terrainNoise;

	terrainNoise.setType(cursed_engine::NoiseType::Perlin);
	terrainNoise.setFrequency(0.06f);

	//terrainNoise.setType(cursed_engine::NoiseType::OpenSimplex2);
	//terrainNoise.setSeed(1); // Todo, random seed!
	//terrainNoise.setFrequency(1.f);
}

void MapGenerator::generateStartArea(TileMap& map, int seed)
{
	MapChunk mapChunk = generateMapChunk(0, 0); // pass seed?

	map.insertMapChunk(std::move(mapChunk));

	// send event map chunk generated...
}

MapChunk MapGenerator::generateMapChunk(int x, int y) const
{
	// handle different layers (water and ground)

	MapChunk mapChunk{ x, y };

	auto& groundLayer = mapChunk.layers[(std::size_t)LayerType::Ground];
	// auto& waterLayer = mapChunk.layers[(std::size_t)LayerType::Water]; - todo, add water to water layer OR dont have different layers? (shore tiles need to contain sand underneath water)

	std::vector<TerrainType> terrainTypes;

	for (int i = 0; i < TileLayer::tileCount; ++i)
	{
		int x = i % TileLayer::width;
		int y = i / TileLayer::width;

		float value = m_settings.terrainNoise.sample((float)x, (float)y);

		terrainTypes.push_back(getTerrainType(value));
		//groundLayer.tileIds[i] = getTerrainType(value);
	}


	// assign correct tile.
	// iterate through all terrain thresholds, check if less, or equal, than

	for (int i = 0; i < terrainTypes.size(); ++i)
	{
		const TerrainType& terrainType = terrainTypes.at(i);
				
		if (terrainType == TerrainType::DeepWater || terrainType == TerrainType::ShallowWater)
		{ 
			groundLayer.tileIds[i] = 3;
		}
		else if (terrainType == TerrainType::Grass || terrainType == TerrainType::Jungle)
		{
			groundLayer.tileIds[i] = 1;
		}
		else if (terrainType == TerrainType::Sand)
		{
			groundLayer.tileIds[i] = 2;
		}

		// TODO; use bitmask?!

		// check if same tile type in all 4 directions...
	}

	//groundLayer.tileIds = std::array<TileId, TileLayer::tileCount>{
	//	1, 1, 1,
	//	1, 1, 1,
	//	1, 1, 1
	//};

	groundLayer.tileSetId = "island_tileset";
	groundLayer.isActive = true;

	return mapChunk;
}

TerrainType MapGenerator::getTerrainType(float value) const
{
	if (value < -0.25f)
	{
		return TerrainType::DeepWater;
	}
	else if (value > -0.25f && value < 0.f) // or just value < 0.f shallow ocean
	{
		return TerrainType::ShallowWater;
	}
	else if (value > 0.f && value < 0.15f) // beach/sand
	{
		return TerrainType::Sand;
	}
	else if (value > 0.15f && value < 0.55f) // grass
	{
		return TerrainType::Grass;
	}
	else if (value > 0.55f && value < 0.75f) // jungle
	{
		return TerrainType::Jungle;
	}
	else // hill
	{
		return TerrainType::Jungle;
	}
}
