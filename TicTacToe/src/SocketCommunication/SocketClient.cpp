#include "SocketClient.hpp"
#include "SocketProxyExceptions.h"

#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace SocketCommunication;

SocketClient* SocketClient::wizardSetup()
{
	std::string ipInput;
	int portInput;
	
	std::cout << "Enter an IP (ex: 127.0.0.1): ";
	std::cin >> ipInput;
	
	std::cout << "Enter a port (ex: 8000): ";
	std::cin >> portInput;
	
	std::cout
		<< "Trying to connect on "
		<< ipInput << ":" << portInput << std::endl;

	auto client = new SocketCommunication::SocketClient();
	try
	{
		client->setup(ipInput.c_str(), portInput);
	}
	catch (const std::exception& e)
	{
		std::cerr << "Failed to setup the client: " << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	
	std::cout << "Successfully connected" << std::endl;
	return client;
}

SocketClient::~SocketClient()
{
	if (m_socket > 0)
	{
		if (close(m_socket) == -1)
		{
			std::cerr
				<< "Failed to close socket: "
				<< strerror(errno) << std::endl;
		}
	}
}

void SocketClient::setup(const char* const ip, const int port)
{
	m_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_socket == -1)
		throw Exceptions::SocketProxyException(strerror(errno));
	
	sockaddr_in serverAddress;
	memset(&serverAddress, '0', sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port);
	
	if (inet_pton(AF_INET, ip, &serverAddress.sin_addr) == -1)
		throw Exceptions::SocketProxyException(strerror(errno));
	
	if (connect(
		m_socket, (sockaddr*)&serverAddress, sizeof(serverAddress)) == -1)
	{
		throw Exceptions::SocketProxyException(strerror(errno));
	}
}

void SocketClient::sendMsg(const std::string msg) const
{
	if (send(m_socket, msg.c_str(), msg.length(), 0) == -1)
		throw Exceptions::SocketProxyException(strerror(errno));
}

std::string SocketClient::receive() const
{
	char buf[1024] = { 0 };
	const auto valread = read(m_socket, buf, sizeof(buf));
	
	if (valread == 0)
		throw Exceptions::EndOfConnection();
	
	if (valread == -1)
		throw Exceptions::SocketProxyException(strerror(errno));
	
	buf[valread] = '\0';
	return std::string(buf);
}
