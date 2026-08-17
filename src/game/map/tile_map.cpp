#include "game/map/tile_map.h"
#include <ranges>

void TileMap::insertMapChunk(MapChunk mapChunk)
{
	m_mapChunks.push_back(std::move(mapChunk));

	// TODO; fix chunk coord initialization! 
	m_coordsToIndex.insert({ChunkCoord{ mapChunk.coords.x, mapChunk.coords.y }, m_mapChunks.size() - 1});
}

bool TileMap::isValidChunk(int x, int y) const noexcept
{
	// check unoredered map instead?

	auto it = std::find_if(m_mapChunks.begin(), m_mapChunks.end(), 
		[=](const MapChunk& mapChunk) 
		{
			const auto& coords = mapChunk.coords;
			return coords.x == x && coords.y == y;
		});

	//for (const auto& mapChunk : m_mapChunks)
	//{
	//	const auto& coords = mapChunk.coords;
	//	if (coords.x == x && coords.y == y)
	//	{
	//		return true;
	//	}
	//}

	return it != m_mapChunks.end();
}

std::vector<const MapChunk*> TileMap::getVisibleMapChunks() const noexcept
{
	auto visible = m_mapChunks
		| std::views::filter([](const MapChunk& mapChunk) { return true; })   // TODO; filter!
		| std::views::transform([](const MapChunk& mapChunk) { return &mapChunk; });

	return std::vector<const MapChunk*>(visible.begin(), visible.end());
}

std::vector<MapChunk*> TileMap::getVisibleMapChunks() noexcept
{
	// const cast?

	auto visible = m_mapChunks 
		| std::views::filter([](const MapChunk& mapChunk) { return true; })   // TODO; filter!
		| std::views::transform([](MapChunk& mapChunk) { return &mapChunk; });

	return std::vector<MapChunk*>(visible.begin(), visible.end());

	//std::vector<MapChunk*> result(visible.begin(), visible.end());

	//return std::vector<MapChunk*>{};
}
