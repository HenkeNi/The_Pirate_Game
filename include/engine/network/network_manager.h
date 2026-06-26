#pragma once
#include "engine/network/socket.h"

namespace cursed_engine
{
	class NetworkManager
	{
	public:
		bool initialize();
		bool shutdown();

		void startServer();
		void stopServer();

		void connectToServer();

		void update();

	private:
		Socket m_socket;
		//Socket m_listenSocket;
		//std::vector<Connection> m_connections;
	};
}