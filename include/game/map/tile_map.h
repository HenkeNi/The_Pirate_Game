#pragma once
#include "game/map/map_types.h"
#include <engine/rendering/render_types.h>
#include <span>
#include <vector>

// or TileMapSettings..
//struct TileMapConfig
//{
//	//static constexpr uint32_t chunkWidth = 32;
//	//static constexpr uint32_t chunkHeight = 32;
//};


struct MapChunk
{
	// position?
	// coords? x / y?
	std::vector<TileId> tileIds;
	// tileid
	cursed_engine::Geometry geometry;

	bool isDirty = true;
	// GEometry
	// dirty
};

// offload chunks? cache coords to map chunk index?
class TileMap
{
public:
	void insertMapChunk(MapChunk chunk); // send event? or mapgenerator?

	// get mapchunk at coords? (x / y or world position)

	[[nodiscard]] std::vector<const MapChunk*> getVisibleMapChunks() const noexcept; // return chunk indicies instead? or forEachChunk...
	[[nodiscard]] std::vector<MapChunk*> getVisibleMapChunks() noexcept;

private:
	//TileMapConfig m_config;
	std::vector<MapChunk> m_mapChunks;
};