#include "Game/TicTacToe.hpp"
#include "SocketCommunication/SocketServer.hpp"
#include "SocketCommunication/SocketClient.hpp"
#include <regex>
#include <iostream>

static SocketCommunication::SocketServer* serverWizard()
{
	std::cout << "Creating the server" << std::endl;
	std::cout << "Enter an integer port. (ex: 8000): ";
	
	int port;
	std::cin >> port;
	
	auto server = new SocketCommunication::SocketServer();
	try
	{
		server->setup(port);
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
		server->startAcceptingConnections();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Failed to connect with others: " << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	std::cout << "Successfully connected" << std::endl;
	
	return server;
}

static SocketCommunication::SocketClient* clientWizard()
{
	std::string ipInput;
	int portInput;
	
	std::cout << "Enter an IP (ex: 127.0.0.1): ";
	std::cin >> ipInput;
	
	std::cout << "Enter a port (ex: 8000): ";
	std::cin >> portInput;
	
	std::cout
		<< "Trying to connect on " << ipInput << ":" << portInput << std::endl;

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

static const int kItsNotAMoveMsg = -1;
static const int kEndOfGame = 1;

static void requestAMove(const Game::TicTacToe& game, int& inX, int& inY)
{
	while (true)
	{
		std::cout<< "Give me a move (ex: 0 1): ";
		std::cin >> inX >> inY;
		
		if (game.isValidMove(inX, inY))
			break;
		
		std::cout << "Invalid move" << std::endl;
	}
}

static std::string formatMoveMsg(
	const char playingChar,
	const int x,
	const int y)
{
	return
		std::string("{move: ") +
		playingChar + " " +
		std::to_string(x) + " " +
		std::to_string(y) + "}";
}

static void informOponentAboutMyMove(
	const SocketCommunication::ISocketProxy& networkProxy,
	const char playingChar,
	const int x,
	const int y)
{
	auto moveMsg = formatMoveMsg(playingChar, x, y);
	try
	{
		networkProxy.sendMsg(moveMsg);
	}
	catch (const std::exception& e)
	{
		std::cerr << "Failed to send move message: " << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
}

/*
** Sends a message in the form: "{move: X 0 2}" if it succeeds.
** Returns end of game if player won.
** Exit if it fails.
*/

int myMoveLogic(
	const SocketCommunication::ISocketProxy& networkProxy,
	Game::TicTacToe& game,
	const char playingChar)
{
	int inX;
	int inY;

	requestAMove(game, inX, inY);
	game.executeMove(playingChar, inX, inY);
	informOponentAboutMyMove(networkProxy, playingChar, inX, inY);
	
	if (game.charIsAWinner(playingChar))
	{
		std::cout << game << std::endl;
		std::cout << "You won" << std::endl;
		return kEndOfGame;
	}
	
	return 0;
}

static std::string getMoveMsgFromOponent(
	const SocketCommunication::ISocketProxy& networkProxy)
{
	try
	{
		return networkProxy.receive();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Connection failed: " << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
}

static int parseMoveMsg(
	Game::TicTacToe& game,
	const std::string& moveMsg)
{
	std::smatch match;
	
	const auto itsAMoveMsg = std::regex_search(
		moveMsg,
		match,
		std::regex("^\\{move: ([XO]) ([0-9]+) ([0-9]+)\\}$"));

	if (itsAMoveMsg)
	{
		const auto oponentCharacter = match[1].str()[0];
		const auto oponentMoveX = std::stoi(match[2].str());
		const auto oponentMoveY = std::stoi(match[3].str());
		
		if (!game.executeMove(oponentCharacter, oponentMoveX, oponentMoveY))
		{
			std::cerr << "Internal move msg transmision error" << std::endl;
			exit(EXIT_FAILURE);
		}
		
		std::cout
			<< "Oponenet move: "
			<< oponentMoveX << " " << oponentMoveY
			<< std::endl;
		
		if (game.charIsAWinner(oponentCharacter))
		{
			std::cout << game << std::endl;
			std::cout << "You lost" << std::endl;
			return kEndOfGame;
		}
		
		return 0;
	}
	else
	{
		return kItsNotAMoveMsg;
	}
}

int oponentMoveLogic(
	const SocketCommunication::ISocketProxy& networkProxy,
	Game::TicTacToe& game)
{
	std::smatch match;
	
	std::cout << "Waiting for the oponent to make a move" << std::endl;
	const std::string networkMsg = getMoveMsgFromOponent(networkProxy);
	const auto moveParseResult = parseMoveMsg(game, networkMsg);
	
	return moveParseResult;
}

static void playAGame(
	const SocketCommunication::ISocketProxy& networkProxy,
	const char playingChar,
	const int gameWidth,
	const int gameHeight,
	bool itsMyMove)
{
	auto game = Game::TicTacToe(gameWidth, gameHeight);

	while (true)
	{
		std::cout << game << "----------------" << std::endl;
		
		if (itsMyMove)
		{
			if (myMoveLogic(networkProxy, game, playingChar) == kEndOfGame)
			{
				break;
			}
		}
		else
		{
			if (oponentMoveLogic(networkProxy, game) == kEndOfGame)
			{
				break;
			}
		}
		itsMyMove = !itsMyMove;
	}
}

void runGameLogic(
	const SocketCommunication::ISocketProxy& networkProxy,
	char playingChar,
	char oponentChar,
	bool itsMyMove,
	const int gameWidth = 3, const int gameHeight = 3)
{
	while (true)
	{
		playAGame(networkProxy, playingChar, gameWidth, gameHeight, itsMyMove);
		std::cout << "<--- New game --->" << std::endl;
		
		std::swap(playingChar, oponentChar);
		itsMyMove = !itsMyMove;
	}
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
	
	SocketCommunication::ISocketProxy* socketProxy;
	char playingChar;
	char oponentChar;
	bool itsMyMove;
	
	if (userInput == "1" || userInput == "host")
	{
		socketProxy = serverWizard();
		playingChar = 'X';
		oponentChar = 'O';
		itsMyMove = true;
	}
	else if (userInput == "2" || userInput == "join")
	{
		socketProxy = clientWizard();
		playingChar = 'O';
		oponentChar = 'X';
		itsMyMove = false;
	}
	else
	{
		std::cerr << "Invalid input..." << std::endl;
		exit(EXIT_FAILURE);
	}
	
	runGameLogic(*socketProxy, playingChar, oponentChar, itsMyMove, 3, 3);
	return 0;
}
