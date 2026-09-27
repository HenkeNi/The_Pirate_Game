#include "game/scenes/scene_loader.h"
#include "game/scenes/scene.h"
#include <engine/utils/json/json_document.h>
#include <engine/utils/json/json_value.h> // TODO; odnt include? shared include for json or include in document?
#include <engine/core/result.h>

#include <engine/ecs/entity/entity_factory.h>
#include <engine/ecs/component/component_registry.h>
#include <engine/core/application.h>
#include <engine/ecs/component/core_components.h>

#include <engine/core/events/event_bus.h>
#include <engine/core/events/events.h>

void SceneLoader::init(SceneContext sceneCtx, ce::ComponentInitContext initCtx, ce::EventBus* eventBus)
{
	m_sceneContext = std::move(sceneCtx); 
	m_componentInitContext = std::move(initCtx);
	//m_componentPostInitContext = std::move(postInitCtx);
	m_eventBus = eventBus;
}

std::unique_ptr<Scene> SceneLoader::load(const SceneMeta& meta)
{
	cursed_engine::JsonDocument document;
	const ce::Result<void> result = document.loadFromFile(meta.path);

	//assert(result.ok() && "Failed to load json document!");

	if (!result.ok())
	{
		// cursed_engine::Logger::logError(message);
		return nullptr;
	}

	std::unique_ptr<Scene> scene = meta.creator(m_sceneContext);

	// TODO; attach physcis world? (specify in json, gravity. ect?)

	auto* componentRegistry = m_sceneContext.componentRegistry;
	//auto* componentRegistry = scene.m_context.componentRegistry;

	if (!componentRegistry)
	{
		cursed_engine::Logger::logError("Invalid Component registry!"); // return result instead?
		return nullptr;
	}

	auto& ecsRegistry = scene->m_registry;

	struct PendingParent
	{
		cursed_engine::EntityHandle childHandle;
		const std::string parent;
	};

	std::unordered_map<std::string, cursed_engine::EntityHandle> idToHandle;
	std::vector<PendingParent> pendingParents;

	for (const auto& entity : document["entities"].asArray())
	{
		std::string entityId = entity["id"].asString(); // rename id as name?

		auto entityHandle = ecsRegistry.createEntity();
		idToHandle.insert({ entityId, entityHandle });

		if (entity.hasMember("parent"))
		{
			pendingParents.emplace_back(entityHandle, entity["parent"].asString());
		}

		// ##################### Maybe find better way of calling post init function?? ###################3

		std::vector<const char*> components;

		// ###############################################################################3

		entity["components"].forEachProperty([&](const char* name, cursed_engine::JsonValue value)
			{
				assert(componentRegistry->contains(name) && "[SceneLoader::loadAssets] - Component Type not registered!"); // TODO; make sure program doesnt crahs if not registered

				components.push_back(name);

				const auto& componentData = componentRegistry->get(name);
				componentData.deserializeFromJson(entityHandle, value, m_componentInitContext);
			});


		ce::PhysicsWorld* physicsWorld = scene->getPhysicsWorld();

		if (!physicsWorld)
			continue; // return instead?


		for (const auto& c : components)
		{
			const auto& componentData = componentRegistry->get(c);

			if (!componentData.postInit)
				continue;

			componentData.postInit(entityHandle, { physicsWorld });
		}
	}

	// -------------------------------- setup relationships ------------------------------------

	for (const auto& pendingParent : pendingParents)
	{
		// Find parent
		auto parentIt = idToHandle.find(pendingParent.parent);
		if (parentIt == idToHandle.end())
		{
			// warning or error?
			cursed_engine::Logger::logError("Error establishing child - parent relationship, parent couldn't be found: " + pendingParent.parent);
			continue;
		}

		// assign / get parents hierarchy component
		auto* parentHierarchyComponent = parentIt->second.tryGetComponent<cursed_engine::HierarchyComponent>(); // check is has component instead?
		if (!parentHierarchyComponent)
		{
			auto result = parentIt->second.attachComponent<cursed_engine::HierarchyComponent>();

			if (!result.second)
			{
				// warning instead?
				cursed_engine::Logger::logError("Failed to attach HierarchyComponent to parent: " + pendingParent.parent);
				continue;
			}

			parentHierarchyComponent = result.first;
		}


		// TODO: Attach child handle to parent hiearchy component...

		cursed_engine::EntityHandle* childHandle = &parentHierarchyComponent->firstChild;

		// recursive function instead?
		while (childHandle && childHandle->isValid())
		{
			auto* childHierarchyComponent = childHandle->tryGetComponent<cursed_engine::HierarchyComponent>();
			assert(childHierarchyComponent && "No valid HierarchyComponent");

			childHandle = &childHierarchyComponent->nextSibling; // overwrites first child?

			//if (!childHierarchyComponent)
			//{
			//	childHierarchyComponent = childHandle->attachComponent<cursed_engine::HierarchyComponent>().first; // valid check?
			//}

			//childHierarchyComponent->parent = parentIt->second;


			// need to ste next sibling pointing to previous?
			// or at least current child shoudl point to nextsiling
		}

		*childHandle = pendingParent.childHandle;

		auto* childHierarchyComponent = childHandle->tryGetComponent<cursed_engine::HierarchyComponent>();

		if (!childHierarchyComponent)
		{
			childHierarchyComponent = childHandle->attachComponent<cursed_engine::HierarchyComponent>().first; // valid check?
		}

		childHierarchyComponent->parent = parentIt->second;
	}





	return scene;
}