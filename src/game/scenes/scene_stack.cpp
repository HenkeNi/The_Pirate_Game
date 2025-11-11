//#include "engine/pch.h"
#include "game/scenes/scene_stack.h"
#include "game/scenes/scene.h"
//#include <algorithm>

SceneStack::SceneStack()
{
}

SceneStack::~SceneStack()
{
}

void SceneStack::push(std::unique_ptr<Scene> scene)
{
	if (!m_stack.empty())
		m_stack.back()->onExit();

	scene->onCreated();

	m_stack.push_back(std::move(scene));
	m_stack.back()->onEnter();
}

void SceneStack::pop()
{
	if (!m_stack.empty()) [[likely]]
	{
		auto& scene = m_stack.back();
		scene->onExit();
		scene->onDestroyed();

		m_stack.pop_back();
	}

	if (!m_stack.empty()) [[likely]]
	{
		m_stack.back()->onEnter();
	}
}

void SceneStack::update(float deltaTime)
{
	if (!m_stack.empty()) [[likely]]
	{
		m_stack.back()->update(deltaTime);
	}
}

void SceneStack::clear()
{
	std::for_each(m_stack.begin(), m_stack.end(),
		[](auto& scene)
		{
			scene->onExit();
			scene->onDestroyed();
		});

	m_stack.clear();
}

//
//class SceneManager {
//public:
//    void LoadSceneAsync(const std::string& sceneName) {
//        // 1. Start async loading
//        m_loadingFuture = std::async(std::launch::async, [this, sceneName]() {
//            return LoadSceneData(sceneName);
//            });
//
//        // 2. Show loading screen
//        m_loadingScreen->Show();
//
//        // 3. Transition state
//        m_currentState = State::Loading;
//    }
//
//    void Update() {
//        switch (m_currentState) {
//        case State::Loading:
//            if (m_loadingFuture.wait_for(0s) == std::future_status::ready) {
//                OnSceneLoaded();
//            }
//            break;
//        case State::Transitioning:
//            UpdateTransition();
//            break;
//        case State::Running:
//            m_currentScene->Update();
//            break;
//        }
//    }
//
//private:
//    enum class State { Loading, Transitioning, Running };
//    State m_currentState;
//    std::future<SceneData> m_loadingFuture;
//    std::unique_ptr<Scene> m_currentScene;
//};