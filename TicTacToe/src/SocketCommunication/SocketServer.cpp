#include "SocketServer.hpp"
#include "SocketProxyExceptions.h"
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

using namespace SocketProxy;

SocketServer::SocketServer()
{
}

SocketServer::~SocketServer()
{
}

void SocketServer::setup(int port)
{
	if ((m_serverFd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		throw Exceptions::SocketProxyException(strerror(errno));
	
	int opt = 1;
	if (setsockopt(
		m_serverFd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) == -1)
	{
		throw Exceptions::SocketProxyException(strerror(errno));
	}
	
	m_address.sin_family = AF_INET;
	m_address.sin_addr.s_addr = INADDR_ANY;
	m_address.sin_port = htons(port);
	
	if (bind(m_serverFd, (sockaddr*)&m_address, sizeof(m_address)) == -1)
		throw Exceptions::SocketProxyException(strerror(errno));
	
	const auto maxConnections = 1;
	if (listen(m_serverFd, maxConnections) == -1)
		throw Exceptions::SocketProxyException(strerror(errno));
}

void SocketServer::startAcceptingConnections()
{
	auto adrlen = sizeof(m_address);
	m_clientSocket =
		accept(m_serverFd, (sockaddr*)&m_address, (socklen_t*)&adrlen);
	
	if (m_clientSocket == -1)
		throw Exceptions::SocketProxyException(strerror(errno));
}

void SocketServer::sendMsg(std::string msg) const
{
	if (send(m_clientSocket, msg.c_str(), msg.length(), 0) == -1)
		throw Exceptions::SocketProxyException(strerror(errno));
}

std::string SocketServer::receive() const
{
	char buf[1024] = { 0 };
	
	const auto valread = read(m_clientSocket, buf, sizeof(buf));
	
	if (valread == 0)
		throw Exceptions::EndOfConnection();
	
	if (valread == -1)
		throw Exceptions::SocketProxyException(strerror(errno));
	
	buf[valread] = '\0';
	return std::string(buf);
}
