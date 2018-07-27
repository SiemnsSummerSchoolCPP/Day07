#pragma once

#include "GameMoveModel.h"
#include "TicTacToe.hpp"
#include "SocketCommunication/ISocketProxy.hpp"

namespace Game
{
	class NetworkTicTacToe
	{
	public:
		bool iAmTheHost() const;
	
		NetworkTicTacToe();
		
		int setup(bool& itsMyMove, char& startingChar, char& oponentChar);
		void playGame(const char playingChar, bool itsMyMove) const;
		void continuousGaming(
			bool itsMyMove,
			char playChar1 = 'X',
			char playChar2 = 'O') const;
		
	private:
		const int m_gameWidth = 3;
		const int m_gameHeight = 3;
		SocketCommunication::ISocketProxy* m_networkProxy;
		
		static std::string formatMoveMsg(const GameMoveModel& model);
		
		int myMoveLogic(Game::TicTacToe& game, char playingChar) const;
		int oponentMoveLogic(Game::TicTacToe& game) const;
		
		void informOponentAboutMyMove(const GameMoveModel& model) const;
	};
}
