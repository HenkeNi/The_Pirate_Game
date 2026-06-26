#include "engine/network/network_manager.h"
#include "engine/core/logger.h"


#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <format>

#pragma comment(lib, "Ws2_32.lib") // link through cmake later? target_link_libraries(Engine PRIVATE Ws2_32)


namespace cursed_engine
{
	bool NetworkManager::initialize()
	{
		constexpr WORD winsockVersion = 0x202;
		WSADATA winsockData;

		const int result = WSAStartup(winsockVersion, &winsockData);

		if (result)
		{
			Logger::logError(std::format("WSAStartup failed. Error: {}", WSAGetLastError()));
			return false;
		}

		return true;
	}

	bool NetworkManager::shutdown()
	{
		const int result = WSACleanup();

		if (result == SOCKET_ERROR)
		{
			Logger::logError(std::format("WSACleanup failed. Error: {}", WSAGetLastError()));
			return false;
		}

		return true;
	}
}