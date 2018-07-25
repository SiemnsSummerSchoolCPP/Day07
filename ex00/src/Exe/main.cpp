#include "Palindrome/Palindrome.h"
#include <iostream>

static inline void checkPalindromic(const char* const str)
{
	std::cout << str << ": ";
	std::cout << (isPalindrome(str) ? "is" : "is not");
	std::cout << " palindromic" << std::endl;
}

int main(const int argc, const char* const* const argv)
{
	if (argc == 1)
	{
		while (std::cin)
		{
			std::string buf;
			
			std::cin >> buf;
			checkPalindromic(buf.c_str());
		}
	}
	else
	{
		for (int i = 1; i < argc; i++)
		{
			checkPalindromic(argv[i]);
		}
	}
	return 0;
}
