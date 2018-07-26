static const int port = 8080;

#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

void initServer()
{
	int serverFd;
	
	if ((serverFd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("Socket failed");
		exit(EXIT_FAILURE);
	}
	
	int opt = 1;
	if (setsockopt(
		serverFd,
		SOL_SOCKET,
		SO_REUSEPORT,
		&opt,
		sizeof(opt)) == -1)
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);
	
	if (bind(serverFd, (sockaddr*)&address, sizeof(address)) == -1)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	
	if (listen(serverFd, 1) == -1)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	
	auto adrlen = sizeof(address);
	auto newSocket = accept(serverFd, (sockaddr*)&address, (socklen_t*)&adrlen);
	if (newSocket == -1)
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}
	
	char buffer[1024] = { 0 };
	while (true)
	{
		const auto valread = read(newSocket, buffer, sizeof(buffer));
		std::cout << "[Server]: " << buffer << std::endl;
		(void)valread;
	}
	const char* msg = "hey from server";
	send(newSocket, msg, strlen(msg), 0);
	std::cout << "Ms sent from server" << std::endl;
}
