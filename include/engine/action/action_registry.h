#pragma once
#include <functional>
#include <string>

namespace cursed_engine
{
	struct Entity;

	class ActionRegistry
	{
	public:
		using ActionID = std::string;
		using Action = std::function<void(Entity entity)>; // accept params?

		template <typename F>
		void registerAction(ActionID id, F&& action);

		void execute(const ActionID& id, Entity e);

		[[nodiscard]] bool isRegistered(const ActionID& id) const noexcept;

	private:
		using Actions = std::unordered_map<ActionID, Action>;

		Actions m_actions;
	};

#pragma region Methods

	template <typename F>
	void ActionRegistry::registerAction(ActionID id, F&& action)
	{
		m_actions.insert_or_assign(std::move(id), std::forward<F>(action));
	}

#pragma endregion
}