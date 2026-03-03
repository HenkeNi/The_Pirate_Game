#include "game/scenes/scene_loader.h"
#include "game/scenes/scene.h"
#include <engine/utils/json/json_document.h>
#include <engine/utils/json/json_value.h> // TODO; odnt include? shared include for json or include in document?


#include <engine/ecs/entity/entity_factory.h>
#include <engine/core/type_registry.hpp>
#include <engine/core/types.h>

void SceneLoader::loadAssets(Scene& scene, const std::filesystem::path& path) const
{
	cursed_engine::JsonDocument document;
	auto [success, message] = document.loadFromFile(path);

	if (!success)
	{
		cursed_engine::Logger::logError(message);
		return;
	}

	auto* componentRegistry = scene.m_context.componentData;
	
	if (!componentRegistry)
	{
		cursed_engine::Logger::logError("Invalid Component Data!");
		return;
	}

	auto& ecsRegistry = scene.m_context.registry;

	for (const auto& entityValue : document["entities"].asArray())
	{
		std::string id = entityValue["id"].asString(); // rename id as name?

		auto entityHandle = ecsRegistry.createEntity();

		entityValue["components"].forEachProperty([&](const char* name, cursed_engine::JsonValue value)
			{
				assert(componentRegistry->isValid(name) && "[SceneLoader::loadAssets] - Component Type not registered!"); // TODO; make sure program doesnt crahs if not registered

				const auto& componentData = componentRegistry->get(name);
				componentData.deserialize(entityHandle, value);
			});
	}
}