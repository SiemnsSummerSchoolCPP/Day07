#pragma once

#include <string>
#include <netinet/in.h>

namespace SocketProxy
{
	class SocketServer
	{
	public:
		SocketServer();
		~SocketServer();
		
		void setup(int port);
		void startAcceptingConnections();
		void sendMsg(std::string msg) const;
		std::string receive() const;
		
	private:
		int m_serverFd = -1;
		int m_clientSocket = -1;
		sockaddr_in m_address;
	};
}
