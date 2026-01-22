#include "game/scenes/scene_loader.h"
#include "game/scenes/scene.h"
#include <engine/utils/json/json_document.h>
#include <engine/ecs/entity/entity_factory.h>
#include <engine/core/type_registry.hpp>
#include <engine/core/types.h>

void SceneLoader::loadAssets(Scene& scene, const std::filesystem::path& path) const
{
	// load all entities here!

	cursed_engine::JsonDocument document;
	auto [success, message] = document.loadFromFile(path);

	if (!success)
	{
		cursed_engine::Logger::logError(message);
		return;
	}

	// auto& componentRegistry = 

	document["entities"].forEachArray([&](cursed_engine::JsonValue value)
		{
			// For each entity...

			std::string id = value["id"].asString(); // rename id as name?

			//auto entityFactory = scene.m_context.entityFactory; // DOES SceneLoader even need to use factory?
			//auto entity = entityFactory->create();
			
			
			auto entityHandle = scene.m_context.registry.createEntity();


			value["components"].forEachObject([&](const char* name, cursed_engine::JsonValue value)
				{
					// assert componentData contains 'name'

					//entityHandle.attachComponent()

					const auto& componentData = scene.m_context.componentData->get(name);
					componentData.deserialize(entityHandle, value);
					// typeregistry.get(name);
					
					// use component info??
					
					// TODO; how to set components value? convert value to prefab????
					

					// how to unpack the data? -> function that accept json data?
					// check how prefab is handled
					int x = 20;
					


					//use the value["test"].asFloat().... ? or check how done in old project if need to handle it more generic

					// Register component types in ECS, map name to load info?

				});

			// auto object = value.
			// std::string id = value.
		});

	//document["entities"].forEach([](std::string name, const rapidjson::Value& value){}) dont expose rapidjson to game?

}