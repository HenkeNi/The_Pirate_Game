#pragma once
#include "engine/network/network_types.h"
#include <winsock2.h>

namespace cursed_engine
{
	//struct NetworkAddress;

	class Socket
	{
	public:
		Socket();
		~Socket();

		bool open(SocketDomain domain, SocketType type);
		void close();

		bool bind(const NetworkAddress& local); // used by server
		bool connect(const NetworkAddress& remote); // used by client
		
		bool listen(int backlog);
		Socket accept();

		int send(const void* data, int size);
		int receive(void* buffer, int size);

		//int sendTo(const NetworkAddress& address, const void* data, std::size_t size);
		//int receiveFrom(const NetworkAddress& address, void* buffer, std::size_t size);
		
		[[nodiscard]] bool isOpen() const noexcept;

	private:
		SocketDomain m_domain;
		SocketType m_type;
		SOCKET m_handle;


		//NetworkAddress m_boundAddress;
		//NetworkAddress m_remoteAddress;
	};
}