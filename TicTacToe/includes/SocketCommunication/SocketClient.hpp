#pragma once

#include <iostream>
#include "ISocketProxy.hpp"

namespace SocketCommunication
{
	class SocketClient : public ISocketProxy
	{
	public:
		static SocketClient* wizardSetup();
	
		void setup(const char* ip, int port);
		
		void sendMsg(std::string msg) const;
		std::string receive() const;
		
	private:
		int m_socket;
	};
}
