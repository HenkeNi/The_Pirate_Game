#include "game/map/tile_map.h"
#include <ranges>

void TileMap::insertMapChunk(MapChunk mapChunk)
{
	m_mapChunks.push_back(std::move(mapChunk));
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
