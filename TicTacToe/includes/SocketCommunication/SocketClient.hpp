#pragma once

#include <iostream>

namespace SocketProxy
{
	class SocketClient
	{
	public:
		void setup(const char* ip, int port);
		void sendMsg(std::string msg) const;
		std::string receive() const;
		
	private:
		int m_socket;
	};
}
