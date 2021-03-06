#pragma once

#include <string>

namespace SocketCommunication
{
	class ISocketProxy
	{
	public:
		virtual ~ISocketProxy() = default;
		
		virtual void sendMsg(std::string msg) const = 0;
		virtual std::string receive() const = 0;
	};
}
