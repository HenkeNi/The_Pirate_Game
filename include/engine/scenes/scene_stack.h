#pragma once
#include "engine/utils/non_copyable.h"
#include "engine/utils/type_traits.h"
#include <vector>
#include <memory>

namespace cursed_engine
{
	// [CONSIDER]; cache scenes in a pool?

    class Scene;

	class SceneStack : private NonCopyable
	{
	public:
		SceneStack();
		~SceneStack();

		SceneStack(SceneStack&&) = default;
		SceneStack& operator=(SceneStack&&) = default;

		template <DerivedFrom<Scene> T, typename... Args>
        void push(Args&&... args);
		void push(std::unique_ptr<Scene> scene);

        void pop();

		template <DerivedFrom<Scene> T, typename... Args>
		void replace(Args&&... args);


		void update(float deltaTime);
		void clear();

	private:
		void loadSceneAsync();

        std::vector<std::unique_ptr<Scene>> m_stack;
	};

#pragma region

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
}