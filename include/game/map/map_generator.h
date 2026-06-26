#pragma once

class TileMap;
struct MapChunk;

class MapGenerator
{
public:
	void generateStartArea(TileMap& map, int seed);
	//[[nodiscard]] TileMap generateMap(int seed) const; // or unique_ptr instead?
	[[nodiscard]] MapChunk generateMapChunk(int x, int y) const;
};