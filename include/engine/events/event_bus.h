#pragma once
#include "engine/core/subsystem.h"
#include "engine/utils/type_utils.h"
#include <functional>
#include <unordered_map>
#include <queue>
#include <any>


#include <vector>
#include <algorithm>
#include <typeindex>

namespace cursed_engine
{
	class EventBus : public Subsystem
	{
	public:
		template <typename Event>
		using Callback = std::function<void(const Event&)>;

		template <typename Event>
		void subscribe(Callback<Event> callback);

		template <typename Event>
		void publish(Event&& event);

		template <typename Event>
		void publishInstantly(Event&& event); // rename emit?

		template <typename Event, typename... Args>
		void publishInstantly(Args&&... args); // Make sure work!

		void dispatchAll();

	private:
		struct QueuedEvent
		{
			std::type_index type;
			std::any data;
		};

		std::queue<QueuedEvent> m_eventQueue;
		std::unordered_map<std::type_index, std::vector<std::function<void(std::any)>>> m_listeners;
	};

#pragma region Methods

	template <typename Event>
	void EventBus::subscribe(Callback<Event> callback)
	{
		auto& vec = m_listeners[getTypeIndex<Event>()];

		vec.push_back([cb = std::move(callback)](const std::any& e) // why any here?
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
	void EventBus::publishInstantly(Event&& event)
	{
		auto& vec = m_listeners[getTypeIndex<Event>()];

		std::for_each(vec.begin(), vec.end(), [&](auto& callback)
			{
				callback(event);
			});

	}

	template <typename Event, typename... Args>
	void EventBus::publishInstantly(Args&&... args)
	{
		publishInstantly(Event{ std::forward<Args>(args)... });
	}

#pragma endregion
}