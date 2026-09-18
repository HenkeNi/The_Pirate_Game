#pragma once
#include "game/scenes/scene_types.h"
#include "game/scenes/scene_loader.h"
#include "game/scenes/scene.h"
#include <engine/utils/containers/registry.hpp>
#include <memory>
#include <optional>
#include <vector>

namespace cursed_engine
{
	struct EngineContext;
	class EventBus;
}

namespace ce = cursed_engine;

class Game;

class SceneManager
{
public:
	SceneManager() = default;
	~SceneManager() = default;

	SceneManager(const SceneManager&) = delete;
	SceneManager(SceneManager&&) = delete;

	SceneManager& operator=(const SceneManager&) = delete;
	SceneManager& operator=(SceneManager&&) = delete;

	//void requestTransition(SceneId id, SceneTransitionType type);
	void requestTransition(SceneName name, SceneTransitionType type);

	[[nodiscard]] std::size_t count() const noexcept;

	[[nodiscard]] bool contains(SceneId) const noexcept;
	[[nodiscard]] bool empty() const noexcept;

private:
	friend class Game;

	void init(const ce::EngineContext& context, SceneRegistry registry);
	void shutdown();

	void update(float deltaTime);
	void applyPendingTransition();

	void push(SceneName name);
	void swap(SceneName name);
	void pop();

	struct SceneTransition
	{
		SceneName name;
		SceneTransitionType transition;
	};

	std::optional<SceneTransition> m_pendingTransition;
	std::vector<std::unique_ptr<Scene>> m_stack;

	SceneRegistry m_registry;
	SceneLoader m_loader;

	ce::EventBus* m_eventBus;
	//ce::Registry<SceneMeta, SceneId> m_registry;
	//std::unordered_map<SceneName, SceneId> m_idToName;

	//SceneContext m_context;
};