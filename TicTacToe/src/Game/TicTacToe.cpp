#include "TicTacToe.hpp"
#include "GameExceptions.hpp"
#include <string.h>
#include <iomanip>

using namespace Game;

const char TicTacToe::filler = ' ';

/*
** Getters & setters.
*/

size_t TicTacToe::getWidth() const { return width; }
size_t TicTacToe::getHeight() const { return height; }
size_t TicTacToe::getWinLength() const { return winLength; }
const char* const* TicTacToe::getBoard() const { return board; }

/*
** Constructors & destructors.
*/

TicTacToe::TicTacToe(const size_t width, const size_t height):
	width(width),
	height(height),
	winLength(std::min(width, height))
{
	if (width < 2 || height < 2)
	{
		throw Exceptions::InvalidGameSize();
	}
	
	board = new char*[height];
	for (int i = 0; i < height; i++)
	{
		board[i] = new char[width];
		memset(board[i], filler, width * sizeof(**board));
	}
}

TicTacToe::~TicTacToe()
{
	if (board)
	{
		for (int i = 0; i < height; i++)
		{
			if (board[i])
			{
				delete[] board[i];
			}
		}
		delete[] board;
	}
}

/*
** Public methods.
*/

bool TicTacToe::isValidMove(const size_t x, const size_t y) const
{
	if (x >= width || y >= height)
	{
		return false;
	}
	
	return board[y][x] == filler;
}

bool TicTacToe::charIsAWinner(const char playerCharacter) const
{
	return
		completedHorizontalLine(playerCharacter) ||
		completedVerticalLine(playerCharacter) ||
		completedObliqueLine(playerCharacter);
}

bool TicTacToe::executeMove(char playerCharacter, size_t posX, size_t posY)
{
	if (!isValidMove(posX, posY))
		return false;
	
	board[posY][posX] = playerCharacter;
	return true;
}

/*
** Private methods.
*/

size_t TicTacToe::countConsecutiveChars(
	const char targetChar,
	const int posX,
	const int posY,
	const int directionX,
	const int directionY) const
{
	if (posX < 0 || posX >= width || posY < 0 || posY >= height)
	{
		return 0;
	}
	
	if (board[posY][posX] != targetChar)
	{
		return 0;
	}
	
	return 1 + countConsecutiveChars(
		targetChar,
		posX + directionX,
		posY + directionY,
		directionX,
		directionY);
}

bool TicTacToe::completedHorizontalLine(const char targetChar) const
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j <= width - winLength; j++)
		{
			if (countConsecutiveChars(targetChar, j, i, 1, 0) == winLength)
				return true;
		}
	}
	return false;
}

bool TicTacToe::completedVerticalLine(const char targetChar) const
{
	for (int j = 0; j < width; j++)
	{
		for (int i = 0; i <= height - winLength; i++)
		{
			if (countConsecutiveChars(targetChar, j, i, 0, 1) == winLength)
				return true;
		}
	}
	return false;
}

bool TicTacToe::completedObliqueLine(char targetChar) const
{
	const auto biggestDif = std::max(height - winLength, width - winLength);
	for (int i = 0; i <= biggestDif; i++)
	{
		for (int j = 0; j <= biggestDif; j++)
		{
			// Left diagonal.
			if (countConsecutiveChars(targetChar, j, i, 1, 1) == winLength)
				return true;
			
			// Right diagonal.
			auto rightDiagonalCount = countConsecutiveChars(
				targetChar,
				width - 1 - j, i,
				-1, 1);
			if (rightDiagonalCount == winLength)
				return true;
		}
	}
	return false;
}

/*
** Operators.
*/

std::ostream& Game::operator<<(std::ostream& o, const TicTacToe& target)
{
	// Print column indexes.
	o << std::setw(2) << "";
	for (size_t j = 0; j < target.width; j++)
	{
		o << std::setw(2) << std::left << (j % 10);
	}
	o << std::endl;
	
	for (size_t i = 0; i < target.height; i++)
	{
		o << std::setw(2) << std::left << (i % 10);
		for (size_t j = 0; j < target.width; j++)
		{
			o << target.board[i][j];
			if (j != target.width - 1)
			{
				o << '|';
			}
		}
		
		// Print a separator line: -+-+-+-
		o << '\n';
		if (i != target.height - 1)
		{
			o << std::setw(2) << "";
			for (int j = 0; j < target.width - 1; j++)
			{
				o << "-+";
			}
			o << "-" << '\n';
		}
		
	}
	o.flush();
	return o;
}
