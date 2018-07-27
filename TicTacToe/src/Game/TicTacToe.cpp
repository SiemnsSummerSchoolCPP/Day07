#include "TicTacToe.hpp"
#include "GameExceptions.hpp"
#include <string.h>
#include <iomanip>

using namespace Game;

const char TicTacToe::filler = ' ';

/*
** Getters & setters.
*/

size_t TicTacToe::getWidth() const { return m_width; }
size_t TicTacToe::getHeight() const { return m_height; }
size_t TicTacToe::getWinLength() const { return winLength; }
const char* const* TicTacToe::getBoard() const { return m_board; }

/*
** Constructors & destructors.
*/

TicTacToe::TicTacToe(const size_t width, const size_t height):
	m_width(width),
	m_height(height),
	winLength(std::min(width, height))
{
	if (width < 2 || height < 2)
	{
		throw Exceptions::InvalidGameSize();
	}
	
	m_board = new char*[height];
	for (int i = 0; i < height; i++)
	{
		m_board[i] = new char[width];
		memset(m_board[i], filler, width * sizeof(**m_board));
	}
}

TicTacToe::~TicTacToe()
{
	if (m_board)
	{
		for (int i = 0; i < m_height; i++)
		{
			if (m_board[i])
			{
				delete[] m_board[i];
			}
		}
		delete[] m_board;
	}
}

/*
** Public methods.
*/

bool TicTacToe::boardIsFilled() const
{
	for (size_t i = 0; i < m_height; i++)
	{
		for (size_t j = 0; j < m_width; j++)
		{
			if (m_board[i][j] == filler)
				return false;
		}
	}
	
	return true;
}

bool TicTacToe::isValidMove(const size_t x, const size_t y) const
{
	if (x >= m_width || y >= m_height)
	{
		return false;
	}
	
	return m_board[y][x] == filler;
}

bool TicTacToe::charIsAWinner(const char playerCharacter) const
{
	return
		completedHorizontalLine(playerCharacter) ||
		completedVerticalLine(playerCharacter) ||
		completedObliqueLine(playerCharacter);
}

bool TicTacToe::executeMove(const GameMoveModel& model)
{
	if (!isValidMove(model.x, model.y))
		return false;
	
	m_board[model.y][model.x] = model.playerChar;
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
	if (posX < 0 || posX >= m_width || posY < 0 || posY >= m_height)
	{
		return 0;
	}
	
	if (m_board[posY][posX] != targetChar)
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
	for (int i = 0; i < m_height; i++)
	{
		for (int j = 0; j <= m_width - winLength; j++)
		{
			if (countConsecutiveChars(targetChar, j, i, 1, 0) == winLength)
				return true;
		}
	}
	return false;
}

bool TicTacToe::completedVerticalLine(const char targetChar) const
{
	for (int j = 0; j < m_width; j++)
	{
		for (int i = 0; i <= m_height - winLength; i++)
		{
			if (countConsecutiveChars(targetChar, j, i, 0, 1) == winLength)
				return true;
		}
	}
	return false;
}

bool TicTacToe::completedObliqueLine(char targetChar) const
{
	const auto biggestDif = std::max(m_height - winLength, m_width - winLength);
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
				m_width - 1 - j, i,
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
	for (size_t j = 0; j < target.m_width; j++)
	{
		o << std::setw(2) << std::left << (j % 10);
	}
	o << std::endl;
	
	for (size_t i = 0; i < target.m_height; i++)
	{
		o << std::setw(2) << std::left << (i % 10);
		for (size_t j = 0; j < target.m_width; j++)
		{
			o << target.m_board[i][j];
			if (j != target.m_width - 1)
			{
				o << '|';
			}
		}
		
		// Print a separator line: -+-+-+-
		o << '\n';
		if (i != target.m_height - 1)
		{
			o << std::setw(2) << "";
			for (int j = 0; j < target.m_width - 1; j++)
			{
				o << "-+";
			}
			o << "-" << '\n';
		}
		
	}
	o.flush();
	return o;
}
