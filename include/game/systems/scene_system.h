#pragma once
#include <engine/ecs/system/system.h>
//#include <optional>
#include <string>

#include <engine/ecs/component/component_registry.h>

namespace cursed_engine
{
	class EventBus;
	//struct ComponentInitContext;
}

class SceneStack;
class SceneFactory;
struct SceneTransitionEvent;

// or scene_transition_system?
class SceneSystem : public cursed_engine::System
{
public:
	SceneSystem(const cursed_engine::ComponentInitContext& ctx, cursed_engine::EventBus* eventBus, SceneStack& sceneStack, SceneFactory& sceneFactory);


	/*void SceneStack::init(cursed_engine::EventBus* eventBus)
	{
		eventBus->subscribe<SceneTransitionEvent>([](SceneTransitionEvent e)
			{
				const auto& scene = e.scene;
				int x = 20;
			});
	}*/

private:
	void transitionToScene(const SceneTransitionEvent& e);


	cursed_engine::ComponentInitContext m_componentContext;
	cursed_engine::EventBus* m_eventBus;
	SceneStack& m_sceneStack;
	SceneFactory& m_sceneFactory;
};