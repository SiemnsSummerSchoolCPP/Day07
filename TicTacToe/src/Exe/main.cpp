#include "Game/NetworkTicTacToe.hpp"
#include <iostream>

int main(const int argc, const char* const* const argv)
{
	auto gameManager = Game::NetworkTicTacToe();
	
	try
	{
		bool itsMyMove;
		char startingChar;
		char oponentChar;
		
		gameManager.setup(itsMyMove, startingChar, oponentChar);
		gameManager.continuousGaming(itsMyMove, startingChar, oponentChar);
	}
	catch (const std::exception& e)
	{
		std::cerr << "Tic-Tac-Toe: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return 0;
}
