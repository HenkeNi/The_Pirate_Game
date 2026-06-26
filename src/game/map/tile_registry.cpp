#include "game/map/tile_registry.h"
#include <engine/core/logger.h>
#include <engine/utils/json/json_document.h>
#include <engine/utils/json/json_value.h>
#include <format>

#define TILE_SIZE 48 // where?

bool TileRegistry::load(const std::filesystem::path& path)
{
	cursed_engine::JsonDocument doc;
	auto result = doc.loadFromFile(path);

	if (!result.success)
	{
		cursed_engine::Logger::logError(std::format("Failed to load tile types from: {}, reason: {}", path.string(), result.errorMessage));
		return false;
	}


	for (const auto& tileset : doc["tilesets"].asArray())
	{
		std::string textureName = tileset["texture"]["name"].asString();
		int width = tileset["texture"]["size"]["width"].asInt();
		int height = tileset["texture"]["size"]["height"].asInt();

		int cols = width / TILE_SIZE;
		//float u = (sprite_index % cols) * tile_size / atlas_width;
		//float v = (sprite_index / cols) * tile_size / atlas_height;

		for (const auto& tile : tileset["tiles"].asArray())
		{
			TileDefinition definition;
			definition.layer = tile["layer"].asInt();
			definition.walkable = tile["walkable"].asBool();

			// calculate sprite index...

			int id = tile["id"].asInt();
			std::string name = tile["name"].asString();
			int column = tile["column"].asInt();
			int row = tile["row"].asInt();

	

			int x = 20;

		}


	}


	return true;
}