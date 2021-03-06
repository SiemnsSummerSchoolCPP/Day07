#include "GameExceptions.hpp"

using namespace Game;

const char* Exceptions::InvalidGameSize::what() const throw()
{
	return "Invalid game dimensions.";
}

const char* Exceptions::InvalidPosition::what() const throw()
{
	return "Invalid position.";
}

const char* Exceptions::FailedToReadInput::what() const throw()
{
	return "Failed to read input.";
}

