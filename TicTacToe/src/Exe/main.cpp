#include "Game/TicTacToe.hpp"
#include <iostream>

void initClient();
void initServer();

int main(const int argc, const char* const* const argv)
{
	if (argc == 1)
	{
		initServer();
	}
	else
	{
		initClient();
	}
//	auto game = Game::TicTacToe(40, 3);
//	std::cout << game << std::endl;
//
//	game.executeMove('O', 1, 0);
////	game.executeMove('O', 2, 1);
//	game.executeMove('O', 3, 2);
//	game.executeMove('G', 2, 1);
//	std::cout << game << std::endl;
//	std::cout << game.charIsAWinner('O') << std::endl;
	return 0;
}
