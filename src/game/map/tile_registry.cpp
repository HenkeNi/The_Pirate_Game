#include "game/map/tile_registry.h"
#include <engine/core/logger.h>
#include <engine/utils/json/json_document.h>
#include <engine/utils/json/json_value.h>
#include <engine/assets/asset_manager.h>
#include <format>

//bool TileRegistry::load(cursed_engine::AssetManager& assetManager, const std::filesystem::path& path)
//{
//	cursed_engine::JsonDocument doc;
//	auto result = doc.loadFromFile(path);
//
//	if (!result.succeeded)
//	{
//		cursed_engine::Logger::logError(std::format("Failed to load tile types from: {}, reason: {}", path.string(), result.message));
//		return false;
//	}
//
//	for (const auto& tileset : doc["tilesets"].asArray())// tileSet correct spelling?
//	{
//		std::string atlasName = tileset["texture"]["name"].asString(); // rename to atlas in json?
//				
//		Tileset set;
//
//		auto handle = assetManager.getAssetHandle<cursed_engine::TextureAtlas>(atlasName);		
//		if (!handle.isValid())
//		{
//			int x = 20;
//		}
//
//		// no need for this? get from atlas?
//		int width = tileset["texture"]["size"]["width"].asInt();
//		int height = tileset["texture"]["size"]["height"].asInt();
//
//		set.textureSize.x = width;
//		set.textureSize.y = height;
//
//		//int cols = width / map_constants::TILE_SIZE;
//
//		//float u = (sprite_index % cols) * tile_size / atlas_width;
//		//float v = (sprite_index / cols) * tile_size / atlas_height;
//
//		for (const auto& tile : tileset["tiles"].asArray())
//		{
//			TileDefinition definition;
//			definition.layer = tile["layer"].asInt();
//			definition.walkable = tile["walkable"].asBool();
//			definition.atlasCoord.x = tile["column"].asInt(); // index instead?
//			definition.atlasCoord.y = tile["row"].asInt();
//			// calculate sprite index...
//
//			int id = tile["id"].asInt();
//			std::string name = tile["name"].asString();
//
//			definition.id = id; // ??
//			definition.spriteIndex = 0; // ???
//
//			if (tile.hasMember("spawnables"))
//			{
//				for (const auto& spawnable : tile["spawnables"].asArray())
//				{
//					std::string id = spawnable["id"].asString();
//					double chance = spawnable["chance"].asDouble();
//
//					definition.spawnables.emplace_back(std::move(id), (float)chance);
//				}
//			}
//
//			set.tileTypes.insert({ id, std::move(definition) });
//		}
//
//		m_tilesets.insert({ atlasName, std::move(set) }); // what key?
//	}
//
//
//	return true;
//}

//const Tileset* TileRegistry::getTileset(const TilesetId& id) const noexcept
//{
//	if (auto it = m_tilesets.find(id); it != m_tilesets.end())
//		return &it->second;
//
//	return nullptr;
//}

//const TileDefinition& TileRegistry::get(TileId id) const
//{
//	// TODO: insert return statement here
//
//	return TileDefinition{};
//}
