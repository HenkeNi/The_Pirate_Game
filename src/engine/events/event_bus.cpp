#include "engine/events/event_bus.h"
#include <algorithm>

namespace cursed_engine
{
	void EventBus::dispatchAll()
	{
		while (!m_eventQueue.empty())
		{
			auto[type, data] = m_eventQueue.front();
			m_eventQueue.pop();
			
			auto it = m_listeners.find(type);
			if (it != m_listeners.end())
			{
				for (auto& fn : it->second) {
					fn(data);
				}

				//auto& listeners = it->second;
				/*std::for_each(listeners.begin(), listeners.end(), [&](std::function<void(const void*)> fnc) 
					{
						fnc(data);
					})*/

			}
		}
	}
}