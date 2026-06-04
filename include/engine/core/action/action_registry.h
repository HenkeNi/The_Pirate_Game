#pragma once
#include "engine/ecs/entity/entity.h" // required for variant
#include <functional>
#include <string>
#include <unordered_map>
#include <variant>

// TODO; functor object instead (Action)? or std::any?
namespace cursed_engine
{
	struct Entity;

	using ActionValue = std::variant<bool, int, double, std::string, Entity>;
	using ActionArgs = std::unordered_map<std::string, ActionValue>;

	class ActionRegistry
	{
	public:
		using ActionID = std::string;
		using Action = std::function<void(const ActionArgs&)>;

		template <typename F>
		void registerAction(ActionID id, F&& action);

		void execute(const ActionID& id, const ActionArgs& args);
		
		/*template <typename... Args>
		void execute(const ActionID& id, Args&&... args);*/

		[[nodiscard]] bool isRegistered(const ActionID& id) const noexcept;

	private:
		using Actions = std::unordered_map<ActionID, Action>;

		Actions m_actions;
	};

#pragma region Definitions

	template <typename F>
	void ActionRegistry::registerAction(ActionID id, F&& action)
	{
		// add staci assert for correct signature?
		m_actions.insert_or_assign(std::move(id), std::forward<F>(action));
	}

	/*template <typename... Args>
	void ActionRegistry::execute(const ActionID& id, Args&&... args)
	{
		assert(isRegistered(id) && "Action is not registered!");

		if (auto it = m_actions.find(id); it != m_actions.end())
		{
			it->second(std::forward);
		}
		else
		{
			Logger::logError("Failed to find action with id: " + id);
		}
	}*/

#pragma endregion
}