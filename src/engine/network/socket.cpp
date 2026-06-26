#include "engine/network/socket.h"
#include "engine/network/network_types.h"
#include "engine/core/logger.h"
#include <format>

#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <ws2tcpip.h>

namespace cursed_engine
{

#pragma region Helpers

	namespace
	{
		constexpr int toAF(SocketDomain domain)
		{
			return (domain == SocketDomain::IPv4) ? AF_INET : AF_INET6;
		}

		constexpr int toType(SocketType type)
		{
			return (type == SocketType::TCP) ? SOCK_STREAM : SOCK_DGRAM;
		}

		constexpr int toProtocol(SocketType type)
		{
			return (type == SocketType::TCP) ? IPPROTO_TCP : IPPROTO_UDP;
		}
	}

#pragma endregion

	Socket::Socket()
		: m_handle{ INVALID_SOCKET }, m_domain{}, m_type{}
	{
	}

	Socket::~Socket()
	{
		close();
	}

	bool Socket::open(SocketDomain domain, SocketType type)
	{
		m_domain = domain;
		m_type = type;

		m_handle = socket(toAF(domain), toType(type), toProtocol(type));

		if (m_handle == INVALID_SOCKET)
		{
			Logger::logError(std::format("[Socket::open] - Failed to open socket. Error: {}", WSAGetLastError()));
			return false;
		}

		return true;
	}

	void Socket::close()
	{
		if (m_handle == INVALID_SOCKET)
			return;

		const int result = closesocket(m_handle);
		
		if (result == SOCKET_ERROR)
		{
			Logger::logError(std::format("[Socket::close] - Failed to close socket. Error: {}", WSAGetLastError()));
			return;
		}

		m_handle = INVALID_SOCKET;
		return;
	}

	bool Socket::bind(const NetworkAddress& local)
	{
		//m_boundAddress = local; // ?

		SOCKADDR_IN localAddress;
		localAddress.sin_family = toAF(m_domain);
		localAddress.sin_port = htons(local.port);

		if (local.ip.empty())
		{
			localAddress.sin_addr.s_addr = INADDR_ANY;
		}
		else
		{
			inet_pton(AF_INET, local.ip.c_str(), &localAddress.sin_addr);
		}

		const int result = ::bind(m_handle, (SOCKADDR*)&localAddress, sizeof(localAddress));

		if (result == SOCKET_ERROR)
		{
			Logger::logError(std::format("Socket bind failed. Error: {}", WSAGetLastError()));
			return false;
		}

		return true;
	}

	bool Socket::connect(const NetworkAddress& remote)
	{
		//m_remoteAddress = remote;

		SOCKADDR_IN serverAddress;
		serverAddress.sin_family = AF_INET;
		serverAddress.sin_port = htons(remote.port);
		serverAddress.sin_addr.S_un.S_addr = inet_addr(remote.ip.c_str());

		const int result = ::connect(m_handle, (sockaddr*)&serverAddress, sizeof(serverAddress));

		if (result == SOCKET_ERROR)
		{
			Logger::logError(std::format("Socket connect failed. Error: {}", WSAGetLastError()));
			return false;
		}

		return true;
	}

	bool Socket::listen(int backlog)
	{
		if (m_handle == INVALID_SOCKET)
		{
			Logger::logError("Trying to listen with an invalid socket!");
			return false;
		}

		const int result = ::listen(m_handle, backlog);
		
		if (result == SOCKET_ERROR)
		{
			Logger::logError(std::format("Socket listening failed. Error: {}", WSAGetLastError()));
			return false;
		}

		return true;
	}
	
	Socket Socket::accept()
	{
		return {};
	}

	int Socket::send(const void* data, int size)
	{
		if (m_type == SocketType::TCP)
		{
			return ::send(m_handle, (const char*)data, size, 0);
		}
		else
		{
			// CORRECT?m,,,
			SOCKADDR_IN address;
			address.sin_family = AF_INET;
			address.sin_port = htons(3000); // PORT??
			address.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); // ADRES??

			return ::sendto(m_handle, (const char*)data, size, 0, (sockaddr*)&address, sizeof(sockaddr_in));
		}
	}

	int Socket::receive(void* buffer, int size) // return bool instead?
	{
		if (m_type == SocketType::TCP)
		{
			return ::recv(m_handle, (char*)buffer, size, 0);
		}
		else // UDP
		{
			sockaddr_in from{};
			int fromLen = sizeof(from);

			const int bytesReceived = ::recvfrom(
				m_handle,
				(char*)buffer,
				size,
				0,
				(sockaddr*)&from,
				&fromLen
			);

			if (bytesReceived == SOCKET_ERROR)
			{
				Logger::logError(std::format("recvfrom returned SOCKET_ERROR. Error: {}", WSAGetLastError()));
				return -1;
			}
			else
			{
				//buffer[bytesReceived] = 0;
				Logger::logInfo(std::format("%d.%d.%d.%d:%d - %s",
					from.sin_addr.S_un.S_un_b.s_b1,
					from.sin_addr.S_un.S_un_b.s_b2,
					from.sin_addr.S_un.S_un_b.s_b3,
					from.sin_addr.S_un.S_un_b.s_b4,
					ntohs(from.sin_port),
					//from.sin_port,
					buffer));
			}

			return 1;
		}
	}
}