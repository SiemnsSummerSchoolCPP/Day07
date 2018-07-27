#pragma once

namespace SocketCommunication
{
	class ISocketProxy
	{
	public:
		virtual void sendMsg(std::string msg) const = 0;
		virtual std::string receive() const = 0;
	};
}
