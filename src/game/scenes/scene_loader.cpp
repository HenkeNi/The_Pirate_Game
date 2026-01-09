#include "game/scenes/scene_loader.h"
#include "game/scenes/scene.h"
#include <engine/utils/json/json_document.h>

void SceneLoader::loadAssets(Scene& scene, const std::filesystem::path& path) const
{
	int x = 20;

	// load all entities here!

	cursed_engine::JsonDocument document;
	auto [success, message] = document.loadFromFile(path);

	if (!success)
	{
		cursed_engine::Logger::logError(message);
		return;
	}

	document["entities"].forEachArray([](cursed_engine::JsonValue value)
		{
			// For each entity...

			std::string id = value["id"].asString();

			value.forEachObject([](const char* name, cursed_engine::JsonValue value)
				{
					// name == "transform", etc....

					// how to handle if object?
					use the value["test"].asFloat().... ? or check how done in old project if need to handle it more generic

					// Register component types in ECS, map name to load info?

				});

			// auto object = value.
			// std::string id = value.


		});

	//document["entities"].forEach([](std::string name, const rapidjson::Value& value){}) dont expose rapidjson to game?

}