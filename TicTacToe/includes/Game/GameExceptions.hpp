#pragma once

#include <exception>

namespace Game { namespace Exceptions {
	struct GameException : public std::exception {};
	
	struct InvalidGameSize : public GameException
	{
		const char* what() const throw();
	};
	
	struct InvalidPosition : public GameException
	{
		const char* what() const throw();
	};
	
	struct FailedToReadInput : public GameException
	{
		const char* what() const throw();
	};
}}

