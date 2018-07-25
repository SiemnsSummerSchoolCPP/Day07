#include "Palindrome.h"
#include <string>

bool isPalindrome(const char* const str)
{
	if (str == nullptr)
	{
		return false;
	}
	
	auto len = strlen(str);
	for (int i = 0; i < len / 2; i++)
	{
		if (*(str + i) != *(str + len - 1 - i))
		{
			return false;
		}
	}
	
	return true;
}
