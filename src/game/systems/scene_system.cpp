#include "game/systems/scene_system.h"
#include "game/scenes/scene_factory.h"
#include "game/scenes/scene_loader.h"
#include "game/events/events.h"
#include <engine/core/events/event_bus.h>

#include "game/scenes/scene_stack.h"
#include "game/scenes/scene.h"

SceneSystem::SceneSystem(const cursed_engine::ComponentInitContext& ctx, cursed_engine::EventBus* eventBus, SceneStack& sceneStack, SceneFactory& sceneFactory)
	: m_componentContext{ ctx }, m_eventBus{ eventBus }, m_sceneStack{ sceneStack }, m_sceneFactory{ sceneFactory }
{
	m_eventBus->subscribe<SceneTransitionEvent>(
		[this](SceneTransitionEvent e)
		{
			transitionToScene(e);
		});
}

void SceneSystem::transitionToScene(const SceneTransitionEvent& e)
{
	// TODO; load and store somewhere...
	//static std::unordered_map<std::string, std::string> idToPaths
	//{
	//	{ "title_scene", "../assets/scenes/title_scene.json"}, // Use assetRoot.string()
	//	{ "settings_scene", "../assets/scenes/settings_scene.json"},
	//	{ "overworld_scene", "../assets/scenes/overworld_scene.json"}
	//};

	/*if (e.scene.empty() && e.transition == "pop")
	{
		m_sceneStack.pop();
		return;
	}*/

	std::unique_ptr<Scene> scene{ nullptr };

	if (!e.scene.empty())
	{
		//std::string path = idToPaths.at(e.scene);

		scene = m_sceneFactory.createScene(e.scene);
		if (!scene)
		{
			// log...
			return;
		}

		SceneLoader loader; // loads scene data?
		loader.loadAssets(*scene, m_sceneFactory.getFilePath(e.scene), m_componentContext);
	}



	//constexpr std::unordered_map<std::string, SceneTransitionType> transitions
	static const std::unordered_map<std::string, SceneTransitionType> transitions
	{
		{ "push", SceneTransitionType::Push },
		{ "pop", SceneTransitionType::Pop },
		{ "replace", SceneTransitionType::Swap } // swpa or replace?
	};

	m_sceneStack.setPending({ std::move(scene), transitions.at(e.transition) });



	//if (e.transition == "push")
	//else if (e.transition == "pop")
	//else if (e.transition == "replace")
	//else if (e.transition == "pop")
	//	m_sceneStack.pop();

	//auto scene = m_sceneFactory.create(name);
	//if (scene == "settings_scene")
	//	int x = 20;

}