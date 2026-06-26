#include "game/scenes/scene_loader.h"
#include "game/scenes/scene.h"
#include <engine/utils/json/json_document.h>
#include <engine/utils/json/json_value.h> // TODO; odnt include? shared include for json or include in document?


#include <engine/ecs/entity/entity_factory.h>
#include <engine/ecs/component/component_registry.h>
#include <engine/core/application.h>
#include <engine/ecs/component/core_components.h>

void SceneLoader::loadAssets(Scene& scene, const std::filesystem::path& path, const cursed_engine::ComponentInitContext& ctx) const
{
	cursed_engine::JsonDocument document;
	auto [success, message] = document.loadFromFile(path);

	if (!success)
	{
		cursed_engine::Logger::logError(message);
		return;
	}

	auto* componentRegistry = scene.m_context.componentRegistry;
	
	if (!componentRegistry)
	{
		cursed_engine::Logger::logError("Invalid Component registry!");
		return;
	}

	auto& ecsRegistry = scene.m_registry;

	// TODO; two phase initialization... 2nd phase connects relationships...

	std::unordered_map<std::string, cursed_engine::Entity> entityIdToEntity;
	std::unordered_map<cursed_engine::Entity, std::string> entityToEntityId;

	for (const auto& entityValue : document["entities"].asArray())
	{
		std::string id = entityValue["id"].asString(); // rename id as name?

		auto entityHandle = ecsRegistry.createEntity();

		entityIdToEntity.insert({ id, entityHandle.getEntity() });
		entityToEntityId.insert({ entityHandle.getEntity(), id });

		entityValue["components"].forEachProperty([&](const char* name, cursed_engine::JsonValue value)
			{
				assert(componentRegistry->isValid(name) && "[SceneLoader::loadAssets] - Component Type not registered!"); // TODO; make sure program doesnt crahs if not registered

				const auto& componentData = componentRegistry->get(name);
				componentData.deserialize(entityHandle, value, ctx);

			});

		// OR DO when creating? (always create parent first?)
		// connect parents to children... 
		
		//auto view = ecsRegistry.view<cursed_engine::ParentComponent>();
		//view.forEach([](cursed_engine::ParentComponent& parentComponent)
		//	{
		//		// Need to know what 
		//		//parentComponent.parent = 

		//	});
	}

	// TODO; create function?
	auto view = ecsRegistry.view<cursed_engine::ParentComponent>();
	view.forEach([&](cursed_engine::ParentComponent& parentComponent)
		{
			const auto& parentIdentifier = parentComponent.parentIdentifier;

			if (auto it = entityIdToEntity.find(parentIdentifier); it != entityIdToEntity.end())
			{
				parentComponent.parent = cursed_engine::EntityHandle(it->second, &ecsRegistry);
			}

		});
}

//void SceneLoader::createEntities(const class JsonArrayView& json, const cursed_engine::ComponentInitContext& ctx) const
//{
//
//}