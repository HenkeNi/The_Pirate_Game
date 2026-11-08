#pragma once
#include "engine/network/network_manager.h"

namespace cursed_engine
{
	class Network
	{
	public:
		bool init();  // accept network settings?
		void shutdown();

	private:
		NetworkManager m_networkManager;
	};
}