#pragma once
#include <functional>
#include <unordered_map>
#include <queue>
#include <typeindex>
#include <any>

namespace cursed_engine
{
	//struct Event;

	// EventDispatcher or EventBus?
	class EventBus
	{
	public:
		template <typename Event>
		using Callback = std::function<void(const Event&)>;

		template <typename Event>
		void subscribe(Callback<Event> callback);

		template <typename Event>
		void publish(Event&& event);

		void process();

	private:
		template <typename Event>
		[[nodiscard]] constexpr std::type_index getTypeIndex() const;

		struct QueuedEvent
		{
			std::type_index type;
			std::any data; // or union
		};

		std::queue<QueuedEvent> m_eventQueue;
		std::unordered_map<std::type_index, std::vector<std::function<void(const void*)>>> m_listeners;
	};

#pragma region Methods

	template <typename Event>
	void EventBus::subscribe(Callback<Event> callback)
	{
		auto& vec = m_listeners[std::type_index(typeid(Event))];

		vec.push_back([cb = std::move(callback)](const std::any& e) 
			{
				cb(std::any_cast<const Event&>(e));
			});
	}

	template <typename Event>
	void EventBus::publish(Event&& event)
	{
		m_eventQueue.push(getTypeIndex<Event>(), std::forward<Event>(event));

	}

	template <typename Event>
	constexpr std::type_index EventBus::getTypeIndex() const
	{
		return std::type_index(typeid(Event));
	}

#pragma endregion
}