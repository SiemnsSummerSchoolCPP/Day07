#include "Game/TicTacToe.hpp"
#include "SocketCommunication/SocketServer.hpp"
#include "SocketCommunication/SocketClient.hpp"
#include <iostream>

static SocketProxy::SocketServer serverWizard()
{
	std::cout << "Creating the server" << std::endl;
	std::cout << "Enter an integer port. (ex: 8000): ";
	
	int port;
	std::cin >> port;
	
	auto server = SocketProxy::SocketServer();
	try
	{
		server.setup(port);
	}
	catch (const std::exception& e)
	{
		std::cerr
			<< "Failed to initalize the server: "
			<< e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	
	std::cout << "Waiting for a player..." << std::endl;
	try
	{
		server.startAcceptingConnections();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Failed to connect with others: " << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	std::cout << "Successfully connected" << std::endl;
	
	return server;
}

void serverChoice()
{
	auto server = serverWizard();
}

static SocketProxy::SocketClient clientWizard()
{
	std::string ipInput;
	int portInput;
	
	std::cout << "Enter an IP (ex: 127.0.0.1): ";
	std::cin >> ipInput;
	
	std::cout << "Enter a port (ex: 8000): ";
	std::cin >> portInput;
	
	std::cout
		<< "Trying to connect on " << ipInput << ":" << portInput << std::endl;

	auto client = SocketProxy::SocketClient();
	try
	{
		client.setup(ipInput.c_str(), portInput);
	}
	catch (const std::exception& e)
	{
		std::cerr << "Failed to setup the client: " << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	
	std::cout << "Successfully connected" << std::endl;
	return client;
}

void clientChoice()
{
	auto client = clientWizard();
}

int main(const int argc, const char* const* const argv)
{
	std::cout << "Welcome to Tic-Tac-Toe" << std::endl;
	std::cout
		<< "1. host" << std::endl
		<< "2. join" << std::endl
		<< "Enter command: ";
	
	std::string userInput;
	std::cin >> userInput;
	
	if (userInput == "1" || userInput == "host")
	{
		serverChoice();
	}
	else if (userInput == "2" || userInput == "join")
	{
		clientChoice();
	}
	else
	{
		std::cerr << "Invalid input..." << std::endl;
		exit(EXIT_FAILURE);
	}
	
//	if (argc == 1)
//	{
//		auto server = SocketProxy::SocketServer();
//		server.setup(port);
//
//		std::string input;
//		try
//		{
//			while (std::cin)
//			{
//				std::cin >> input;
//				server.sendMsg(input);
//			}
//		}
//		catch (const std::exception& e)
//		{
//			std::cerr << e.what() << std::endl;
//		}
//	}
//	else
//	{
//		try
//		{
//			auto client = SocketProxy::SocketClient();
//			client.setup("127.0.0.1", port);
//
//			while (true)
//			{
//				std::cout << "Client: " << client.receive() << std::endl;
//			}
//		}
//		catch (const std::exception& e)
//		{
//			std::cerr << e.what() << std::endl;
//		}
//	}

	return 0;
}
