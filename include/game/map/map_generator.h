#pragma once
#include <engine/math/noise.h>
#include <filesystem>

class TileMap;
struct MapChunk;

struct MapGeneratorSettings
{
	float oceanLevel = -0.20f;
	float beachLevel = -0.05f;
	float grassLevel = 0.45f;
	float jungleLevel = 0.45f;
	
	cursed_engine::Noise terrainNoise; // change to heightnoise?
	// temperature noise?
	// moisture noise?
};

class MapGenerator
{
public:
	MapGenerator();

	bool load(const std::filesystem::path& path);
	void generateStartArea(TileMap& map, int seed);
	//[[nodiscard]] TileMap generateMap(int seed) const; // or unique_ptr instead?
	[[nodiscard]] MapChunk generateMapChunk(int x, int y) const;

private:
	MapGeneratorSettings m_settings;
};