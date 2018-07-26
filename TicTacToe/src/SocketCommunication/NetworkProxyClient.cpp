#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

const int port = 8080;

void initClient()
{
	const int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1)
	{
		perror("Socket creation error");
		exit(EXIT_FAILURE);
	}
	
	sockaddr_in address;
	sockaddr_in serverAddress;
	memset(&address, '0', sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port);
	
	if (inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr) == -1)
	{
		perror("Invalid address");
		exit(EXIT_FAILURE);
	}
	
	if (connect(sock, (sockaddr*)&serverAddress, sizeof(serverAddress)) == -1)
	{
		perror("Connection failed");
		exit(EXIT_FAILURE);
	}
	
	const char* msg = "Hello from CLIENT!";
	send(sock, msg, strlen(msg), 0);
	send(sock, msg, strlen(msg), 0);
	std::cout << "msg sent from client" << std::endl;
	
	char buf[1024] = { 0 };
	const auto valread = read(sock, buf, sizeof(buf));
	(void)valread;
	
	std::cout << "[Client]: " << buf << std::endl;
}
