#pragma once
#include "engine/utils/non_copyable.h"
#include "engine/utils/type_traits.h"
#include "engine/utils/type_utils.h"
#include "game/scenes/scene_loader.h"
#include <vector>
#include <memory>
#include <typeindex>
#include <unordered_map>
// [CONSIDER]; cache scenes in a pool?
// Rename SceneManager?

// Register scenes? push path to id?

class Scene;

class SceneStack : private cursed_engine::NonCopyable
{
public:
	SceneStack();
	~SceneStack();

	SceneStack(SceneStack&&) = default;
	SceneStack& operator=(SceneStack&&) = default;

	//template <DerivedFrom<Scene> T>
	//void registerScene(std::string id, std::filesystem::path path); // rename? force user to specify path?
	void addPath(std::string sceneID, std::filesystem::path path);

	template <DerivedFrom<Scene> T, typename... Args>
	void push(Args&&... args);
	// push(std::string)?
	void push(std::unique_ptr<Scene> scene);

	void pop();

	template <DerivedFrom<Scene> T, typename... Args>
	void replace(Args&&... args);

	void update(float deltaTime);
	void clear();

private:
	//void loadAsync();

	std::vector<std::unique_ptr<Scene>> m_stack;
	SceneLoader m_sceneLoader;

	std::unordered_map<std::string, std::filesystem::path> m_idToPaths;

	//std::unordered_map<std::type_index, std::filesystem::path> m_typesToPaths; // use scene id instead?
	//std::unordered_map<std::string, std::type_index> m_namesToTypes;
	
	// Scene context? (engine context)?
	// map id (string) to type index?

	// type index to path?
};

#pragma region

//template <DerivedFrom<Scene> T>
//void SceneStack::registerScene(std::string id, std::filesystem::path path)
//{
//	m_idToPaths.insert({ std::move(id), std::move(path) });
//	//auto type = cursed_engine::getTypeIndex<T>();
//
//	//m_namesToTypes.insert({ name, type });
//	//m_typesToPaths.insert({ type, path });
//}

template <DerivedFrom<Scene> T, typename... Args>
void SceneStack::push(Args&&... args)
{
	push(std::make_unique<T>(std::forward<Args>(args)...));
}

template <DerivedFrom<Scene> T, typename... Args>
void SceneStack::replace(Args&&... args)
{
	pop();
	push<T>(std::forward<Args>(args)...);
}

#pragma endregion