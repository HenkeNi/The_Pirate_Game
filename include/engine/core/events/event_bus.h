#pragma once
#include "engine/utils/utils.h"
#include <functional>
#include <unordered_map>
#include <queue>
#include <any>


#include <vector>
#include <algorithm>
#include <typeindex>

namespace cursed_engine
{
	class EventBus
	{
	public:
		template <typename Event>
		using Callback = std::function<void(const Event&)>;

		template <typename Event>
		void subscribe(Callback<Event> callback); // chnage?

		// TODO; unsubscribe?

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

#pragma region Definitions

	template <typename Event>
	void EventBus::subscribe(Callback<Event> callback)
	{
		auto& vec = m_listeners[utils::getTypeIndex<Event>()];

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
		if (auto it = m_listeners.find(utils::getTypeIndex<Event>()); it != m_listeners.end())
		{
			auto& listeners = it->second;

			std::for_each(listeners.begin(), listeners.end(), [&](auto& callback)
				{
					callback(event);
				});
		}
		//auto& vec = m_listeners[utils::getTypeIndex<Event>()];

		//std::for_each(vec.begin(), vec.end(), [&](auto& callback)
		//	{
		//		callback(event);
		//	});
	}

	template <typename Event, typename... Args>
	void EventBus::publishInstantly(Args&&... args)
	{
		publishInstantly(Event{ std::forward<Args>(args)... });
	}

#pragma endregion
}