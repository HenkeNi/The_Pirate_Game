#pragma once
#include "game/map/map_types.h"
#include <engine/rendering/render_types.h>
#include <array>
#include <vector>

enum class LayerType
{
	Ground, 
	Water,
	Structure,
	Decoration,
	Count
};

struct TileLayer
{
	static constexpr int width = 32;
	static constexpr int height = 32;
	static constexpr int tileCount = width * height;

	std::array<TileId, tileCount> tileIds;
	cursed_engine::Geometry geometry;
	TileSetId tileSetId;
	
	bool isActive = false;
	bool isDirty = true; // mutable?
};

struct MapChunk
{
	std::array<TileLayer, (std::size_t)LayerType::Count> layers;
	cursed_engine::IVec2 coords;

	// position?
	// aabb?
	// helper functions? at? overload[] oeprator?
};

inline cursed_engine::IVec2 getWorldPosition(const MapChunk& mapChunk)
{
	assert(false && "Not tested!");

	cursed_engine::IVec2 worldPosition;
	worldPosition.x = mapChunk.coords.x * (TileLayer::width * map_constants::TILE_SIZE);
	worldPosition.y = mapChunk.coords.y * (TileLayer::height * map_constants::TILE_SIZE);

	return worldPosition;
}

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