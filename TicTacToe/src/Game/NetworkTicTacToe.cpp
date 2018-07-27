#include "NetworkTicTacToe.hpp"
#include "TicTacToe.hpp"
#include "SocketCommunication/SocketClient.hpp"
#include "SocketCommunication/SocketServer.hpp"

#include <iostream>
#include <regex>

using namespace Game;

static const int endOfGameMark = 1;

bool NetworkTicTacToe::iAmTheHost() const
{
	auto const host =
		dynamic_cast<SocketCommunication::SocketServer*>(m_networkProxy);
	
	return host != nullptr;
}

NetworkTicTacToe::NetworkTicTacToe()
{
}

int NetworkTicTacToe::setup(
	bool& itsMyMove,
	char& startingChar,
	char& oponentChar)
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
		m_networkProxy = SocketCommunication::SocketServer::wizardSetup();
		startingChar = 'X';
		oponentChar = 'O';
		itsMyMove = true;
	}
	else if (userInput == "2" || userInput == "join")
	{
		m_networkProxy = SocketCommunication::SocketClient::wizardSetup();
		startingChar = 'O';
		oponentChar = 'X';
		itsMyMove = false;
	}
	else
	{
		std::cerr << "Invalid input..." << std::endl;
		return -1;
	}
	
	return 0;
}

void NetworkTicTacToe::playGame(const char playingChar, bool itsMyMove) const
{
	auto game = Game::TicTacToe(m_gameWidth, m_gameHeight);

	while (true)
	{
		std::cout << game << "----------------" << std::endl;
		if (itsMyMove)
		{
			if (myMoveLogic(game, playingChar) == endOfGameMark)
			{
				break;
			}
		}
		else
		{
			if (oponentMoveLogic(game) == endOfGameMark)
			{
				break;
			}
		}
		
		if (game.boardIsFilled())
		{
			std::cout << game << std::endl;
			std::cout << "It's a draw" << std::endl;
			break;
		}
		
		itsMyMove = !itsMyMove;
	}
}

void NetworkTicTacToe::continuousGaming(
	bool itsMyMove,
	char playChar1,
	char playChar2) const
{
	while (true)
	{
		std::cout << "<--- New game --->" << std::endl;
		playGame(playChar1, itsMyMove);
		
		std::swap(playChar1, playChar2);
		itsMyMove = !itsMyMove;
	}
}

/*
** Private static members
*/

std::string NetworkTicTacToe::formatMoveMsg(const GameMoveModel& model)
{
	return
		std::string("{move: ") +
		model.playerChar + " " +
		std::to_string(model.x) + " " +
		std::to_string(model.y) + "}";
}

/*
** Private members
*/

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

void NetworkTicTacToe::informOponentAboutMyMove(
	const GameMoveModel& model) const
{
	auto moveMsg = NetworkTicTacToe::formatMoveMsg(model);
	m_networkProxy->sendMsg(moveMsg);
}

int NetworkTicTacToe::myMoveLogic(
	Game::TicTacToe& game,
	const char playingChar) const
{
	GameMoveModel model;
	
	model.playerChar = playingChar;
	requestAMove(game, model.x, model.y);
	
	game.executeMove(model);
	informOponentAboutMyMove(model);
	
	if (game.charIsAWinner(model.playerChar))
	{
		std::cout << game << std::endl;
		std::cout << "You won!" << std::endl;
		return endOfGameMark;
	}
	
	return 0;
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
		GameMoveModel model;
		
		model.playerChar = match[1].str()[0];
		model.x = std::stoi(match[2].str());
		model.y = std::stoi(match[3].str());
		
		if (!game.executeMove(model))
		{
			std::cerr << "Internal move msg transmision error" << std::endl;
			exit(EXIT_FAILURE);
		}
		
		std::cout
			<< "Oponenet move: " << model.x << " " << model.y << std::endl;
		
		if (game.charIsAWinner(model.playerChar))
		{
			std::cout << game << std::endl;
			std::cout << "You lost" << std::endl;
			return endOfGameMark;
		}
		
		return 0;
	}
	else
	{
		return -1;
	}
}

int NetworkTicTacToe::oponentMoveLogic(Game::TicTacToe& game) const
{
	std::cout << "Waiting for the oponent to make a move" << std::endl;
	const auto networkMsg = m_networkProxy->receive();
	
	const auto moveParseResult = parseMoveMsg(game, networkMsg);
	
	return moveParseResult;
}
