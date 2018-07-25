#pragma once

#include <stddef.h>
#include <ostream>

namespace Game
{
	class TicTacToe
	{
	public:
		static const char filler;
	
		size_t getWidth() const;
		size_t getHeight() const;
		size_t getWinLength() const;
		const char* const* getBoard() const;
		
		TicTacToe(size_t width = 3, size_t height = 3);
		~TicTacToe();
		
		bool isValidMove(size_t x, size_t y) const;
		bool charIsAWinner(char playerCharacter) const;
		bool executeMove(char playerCharacter, size_t posX, size_t posY);

		friend std::ostream& operator<<(
			std::ostream& o,
			const TicTacToe& target
		);

	private:
		const size_t width;
		const size_t height;
		const size_t winLength;
		char** board = nullptr;
		
		bool completedHorizontalLine(char targetChar) const;
		bool completedVerticalLine(char targetChar) const;
		bool completedObliqueLine(char targetChar) const;
		
		size_t countConsecutiveChars(
			char targetChar,
			int posX, int posY,
			int directionX, int directionY) const;
	};
}
