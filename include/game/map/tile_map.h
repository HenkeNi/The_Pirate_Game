#pragma once
#include "game/map/map_types.h"
#include "engine/utils/utils.h"
#include <engine/rendering/render_types.h>
#include <array>
#include <vector>
#include <unordered_map>

enum class LayerType
{
	Water,
	Ground,
	Structure,
	Decoration,
	Count
};

// have map chunk return (or layer) individual tiles?
//class Row
//{
//public:
//	const TileId& operator[](std::size_t x) const;
//	TileId& operator[](std::size_t x);
//};


struct TileLayer
{
	static constexpr int width = 32;
	static constexpr int height = 32;
	static constexpr int tileCount = width * height;

	// or enums? can't read tile tyeps from json then...
	std::array<TileId, tileCount> tileIds; // maybe store this in mapchunk instead? since only one tile id per tile...
	cursed_engine::Geometry geometry;
	TileSetId tileSetId;

	bool isActive = false;
	bool isDirty = true; // mutable?
};

struct MapChunk
{
	MapChunk() = default;
	MapChunk(int x, int y)
		: coords{ x, y }
	{
	}

	std::array<TileLayer, (std::size_t)LayerType::Count> layers;
	cursed_engine::IVec2 coords;



	// position?
	// aabb?
	// helper functions? at? overload[] oeprator?
};

inline std::pair<int, int> getMapChunkCoordinatesFromWorldPosition(const cursed_engine::FVec2& position)
{
	constexpr int chunkWidth = TileLayer::width * map_constants::TILE_SIZE;
	constexpr int chunkHeight = TileLayer::height * map_constants::TILE_SIZE;

	// get coordinates from position...
	return { static_cast<int>(std::floor(position.x / chunkWidth)),
		static_cast<int>(std::floor(position.y / chunkHeight)) 
	};
}

inline cursed_engine::IVec2 getWorldPosition(const MapChunk& mapChunk)
{
	// assert(false && "Not tested!");

	cursed_engine::IVec2 worldPosition;
	worldPosition.x = mapChunk.coords.x * (TileLayer::width * map_constants::TILE_SIZE);
	worldPosition.y = mapChunk.coords.y * (TileLayer::height * map_constants::TILE_SIZE);

	return worldPosition;
}


struct ChunkCoord
{
	int x;
	int y;

	[[nodiscard]] bool operator==(const ChunkCoord&) const = default;
};

template<>
struct std::hash<ChunkCoord>
{
	std::size_t operator()(const ChunkCoord& coord) const noexcept
	{
		using namespace cursed_engine::utils::hash;

		std::size_t h = 0;

		hashCombine(h, coord.x);
		hashCombine(h, coord.y);

		return h;
	}
};


// offload chunks? cache coords to map chunk index?
class TileMap
{
public:
	void insertMapChunk(MapChunk chunk); // send event? or mapgenerator?

	[[nodiscard]] bool isValidChunk(int x, int y) const noexcept;

	[[nodiscard]] const MapChunk* getChunkAtPosition(int x, int y) const noexcept
	{
		for (const auto& mapChunk : m_mapChunks)
		{
			int width = TileLayer::width * map_constants::TILE_SIZE;
			int height = TileLayer::height * map_constants::TILE_SIZE;

			cursed_engine::IVec2 worldPosition;
			worldPosition.x = mapChunk.coords.x * width;
			worldPosition.y = mapChunk.coords.y * height;

			// use AABB collision
			if (x >= worldPosition.x && y >= worldPosition.y &&
				x <= worldPosition.x + width && y <= worldPosition.y + height)
			{
				return &mapChunk;
			}

		}
		return nullptr;
	}

	[[nodiscard]] const MapChunk* getChunkAtCoords(int x, int y) const
	{
		if (auto it = m_coordsToIndex.find(ChunkCoord{ x, y }); it != m_coordsToIndex.end())
		{
			std::size_t index = it->second;
			if (index >= 0 && index < m_mapChunks.size())
			{
				return &m_mapChunks.at(index);
			}
		}

		return nullptr;
	}

	// get mapchunk at coords? (x / y or world position)

	/*cursed_engine::IVec2 getMapChunkPosition(int x, int y) const
	{
		const auto mapChunkWidth = map_constants::TILE_SIZE * TileLayer::width;
		const auto mapChunkHeight = map_constants::TILE_SIZE * TileLayer::height;

		return {
			x * MAP_CHUNK_WIDTH * m_tileSize,
			y * MAP_CHUNK_HEIGHT * m_tileSize
		};
	}*/


	[[nodiscard]] std::vector<const MapChunk*> getVisibleMapChunks() const noexcept; // return chunk indicies instead? or forEachChunk...
	[[nodiscard]] std::vector<MapChunk*> getVisibleMapChunks() noexcept;

private:
	//TileMapConfig m_config;
	std::vector<MapChunk> m_mapChunks;
	std::unordered_map<ChunkCoord, std::size_t> m_coordsToIndex;
};