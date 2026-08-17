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
		cursed_engine::Logger::logError("Invalid Component registry!"); // return result instead?
		return;
	}

	auto& ecsRegistry = scene.m_registry;

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

		entity["components"].forEachProperty([&](const char* name, cursed_engine::JsonValue value)
			{
				assert(componentRegistry->isValid(name) && "[SceneLoader::loadAssets] - Component Type not registered!"); // TODO; make sure program doesnt crahs if not registered

				const auto& componentData = componentRegistry->get(name);
				componentData.deserialize(entityHandle, value, ctx);
			});
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

		// set next sibling? 

		//*childHandle = pendingParent.childHandle;
	}

	//for (const auto& [parent, children] : parentIdToChildrenIds)
	//{
	//	if (auto it = idToHandle.find(parent); it != idToHandle.end())
	//	{
	//		auto* parentHierarchyComponent = it->second.tryGetComponent<cursed_engine::HierarchyComponent>();
	//		if (!parentHierarchyComponent)
	//		{
	//			// log error/warning...
	//			continue;
	//		}

	//		for (const auto& child : children)
	//		{
	//			//if (auto& firstChild = parentHierarchyComponent->firstChild; !firstChild.isValid())

	//			auto& firstChildHandle = parentHierarchyComponent->firstChild;

	//			if (!firstChildHandle.isValid())
	//			{
	//				// make function? get child?
	//				if (auto childIt = entityIdToEntityHandle.find(child); childIt != entityIdToEntityHandle.end())
	//				{
	//					parentHierarchyComponent->firstChild = childIt->second; // or handle
	//					// also update childs hei
	//					childIt->second.tryGetComponent<cursed_engine::HierarchyComponent>()->parent = it->second;
	//				}
	//			}
	//			else // maybe do this in if and let current if be the else? 
	//			{

	//				// TODO; remember to update child's hiearacrchy as well`!
	//				auto* childHierarchyComponent = firstChildHandle.tryGetComponent<cursed_engine::HierarchyComponent>();

	//				while (childHierarchyComponent->nextSibling.isValid())
	//				{
	//					auto nextChildHandle = childHierarchyComponent->nextSibling;
	//					childHierarchyComponent = nextChildHandle.tryGetComponent<cursed_engine::HierarchyComponent>();
	//				}

	//				assert(!childHierarchyComponent->nextSibling.isValid() && "ERROR?");

	//				if (auto childIt = entityIdToEntityHandle.find(child); childIt != entityIdToEntityHandle.end())
	//				{
	//					childHierarchyComponent->nextSibling = childIt->second;
	//				}

	//				/*while (true)
	//				{
	//					auto& childHierarchyComponent = childHandle.getComponent<cursed_engine::HierarchyComponent>();
	//					
	//					auto& nextSibling = childHierarchyComponent.nextSibling;
	//					if (!nextSibling.isValid())
	//					{
	//						if (auto childIt = entityIdToEntityHandle.find(child); childIt != entityIdToEntityHandle.end())
	//						{
	//							nextSibling = childIt->second;
	//							break;
	//						}
	//					}
	//				}*/
	//			}
	//		}


	//		//hierarchyComponent.parent do reveresed? have parent attach children / siblings? store paretn in json under "core data" not component data
	//	}

	//	// log errror?!
	//	
	//}



	//std::unordered_map<std::string, std::string> entityToParent; // store relationship?????




}

//void SceneLoader::createEntities(const class JsonArrayView& json, const cursed_engine::ComponentInitContext& ctx) const
//{
//
//}