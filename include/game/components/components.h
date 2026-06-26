#pragma once
#include <engine/rendering/render_types.h>
#include <array>

constexpr std::size_t tileDimension = 10;

struct TileDefinition
{	
};

// or tile type?
struct Tile
{
	cursed_engine::FVec2 textureCoords[4]; 
	// bool can see through?
	// can move?
};

// tile registry
//std::vector<TileType> tileTypes;

//struct TileType
//{
//	TextureID texture;
//	bool walkable;
//	bool opaque;
//};

using TileID = uint16_t;
struct TileLayer
{
	//std::array<Tile, tileDimension * tileDimension> tiles; -- dont store actual tiles...
	std::array<TileID, tileDimension * tileDimension> tiles;
};

// maybe map chunks shouldnt be components?
struct MapChunkComponent
{
	TileLayer ground;
	TileLayer water;

	cursed_engine::Geometry geometry;
	bool isDirty = true;

	// Decoration?
	 
	//read from json instead?
	//static constexpr int width = 10;
	//static constexpr int height = 10;

	//static constexpr int tileSize = 5; // not here!?

	//constexpr int ChunkSize = 32;
	// map of tiles?
	// dimensions?
};