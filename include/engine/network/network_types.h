#pragma once
#include <string>

namespace cursed_engine
{
	/*struct NetworkAddress
	{
		NetworkAddress(const char* ip, uint16_t port)
			: port{ port }
		{

		}

		uint32_t ip = INADDR_ANY;
		uint16_t port = 0;
	};*/

	//struct NetworkAddress
	//{
	//	std::string ip;
	//	int port;
	//};

	struct NetworkAddress
	{
		std::string ip;
		int port;
	};

	enum class SocketDomain
	{
		IPv4,
		IPv6
	};

	enum class SocketType
	{
		TCP,
		UDP
	};
}