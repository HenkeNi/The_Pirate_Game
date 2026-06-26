#include "engine/modules/network_module.h"
#include "engine/core/logger.h"
#include <format>

#include <thread>
#include <chrono>

//#define WIN32_LEAN_AND_MEAN // nned this if including windows.h - only include once?

#include <winsock2.h>
#include <ws2tcpip.h>
#include <ntddndis.h>

//#pragma comment(lib, "Ws2_32.lib") // link through cmake later? target_link_libraries(Engine PRIVATE Ws2_32)
/*
* if(WIN32)
	target_link_libraries(MyEngine PRIVATE Ws2_32)
endif()
*/

#include "engine/network/socket.h"


namespace cursed_engine
{
	constexpr std::size_t SOCKET_BUFFER_SIZE = 1024;

	void serverTest()
	{
		//Socket socket;
		//socket.open();

		Socket serverSocket;

		serverSocket.open(SocketDomain::IPv4, SocketType::TCP);
		serverSocket.bind(NetworkAddress{ "0.0.0.0", 3000 });
		serverSocket.listen(10); // 10 = pending requests

		char buffer[SOCKET_BUFFER_SIZE];
		serverSocket.receive(buffer, SOCKET_BUFFER_SIZE);

		//SOCKADDR_IN localAddress;
		//localAddress.sin_family = AF_INET;
		//localAddress.sin_port = htons(3000);
		//localAddress.sin_addr.s_addr = INADDR_ANY;

		//if (bind(socket, (SOCKADDR*)&localAddress, sizeof(localAddress)) == SOCKET_ERROR)
		/*{
			Logger::logError(std::format("Bind failed. Error: {}", WSAGetLastError()));
			return;
		}*/

		Logger::logInfo("Waiting for socket message");

		/*char buffer[SOCKET_BUFFER_SIZE];
		int flags = 0;
		SOCKADDR_IN from;
		int fromSize = sizeof(from);*/

		// use try cahtch?
		//int bytesReceived = recvfrom(socket, buffer, SOCKET_BUFFER_SIZE, flags, (SOCKADDR*)&from, &fromSize);

		//if (bytesReceived == SOCKET_ERROR)
		//{
		//	Logger::logError(std::format("recvfrom returned SOCKET_ERROR. Error: {}", WSAGetLastError()));
		//	return;
		//}
		//else
		//{
		//	buffer[bytesReceived] = 0;
		//	Logger::logInfo(std::format("%d.%d.%d.%d:%d - %s",
		//		from.sin_addr.S_un.S_un_b.s_b1,
		//		from.sin_addr.S_un.S_un_b.s_b2,
		//		from.sin_addr.S_un.S_un_b.s_b3,
		//		from.sin_addr.S_un.S_un_b.s_b4,
		//		ntohs(from.sin_port),
		//		//from.sin_port,
		//		buffer));
		//}


		int x = 20;
	}

	void clientTest()
	{
		Socket clientSocket;
		clientSocket.open(SocketDomain::IPv4, SocketType::TCP);

		clientSocket.connect(NetworkAddress{ "127.0.0.1" , 3000 });

		// clientSocket.accept();

		//SOCKADDR_IN serverAddress;
		//serverAddress.sin_family = AF_INET;
		//serverAddress.sin_port = htons(3000);
		//serverAddress.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

		char message[5];
		gets_s(message, 5);

		clientSocket.send(message, 5);

		//int flags = 0;
		//if (sendto(socket, message, strlen(message), flags, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		//{
		//	Logger::logError(std::format("recvfrom returned SOCKET_ERROR. Error: {}", WSAGetLastError()));
		//	return;
		//}
		//else
		//{
		//	Logger::logInfo("Sent to socket");
		//}
	}

	bool NetworkModule::init()
	{
		if (!m_networkManager.initialize())
		{
			// log?
			return false;
		}

		//////////////////////////////////////// CREATE
		//int addressFamily = AF_INET;
		//int type = SOCK_DGRAM;
		//int protocol = IPPROTO_UDP;
		//SOCKET sock = socket(addressFamily, type, protocol);
		////////////////////////////////////////

		//if (sock == INVALID_SOCKET)
		//{
		//	Logger::logError(std::format("Socket failed. Error: {}", WSAGetLastError()));
		//	return false;
		//}

		std::thread server{
			[=]()
			{
				serverTest();
			} };
		server.detach();

		std::this_thread::sleep_for(std::chrono::milliseconds(1000));

		std::thread client{
			[=]()
			{
				clientTest();
			}
		};


		client.detach();


		return true;
	}

	void NetworkModule::shutdown()
	{
		m_networkManager.shutdown();
	}
}