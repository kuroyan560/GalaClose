#include "Console.h"

void Console::PrintfR(std::string Str)
{
	printf("\x1b[41m%s\x1b[m\n", Str.c_str());
}

void Console::PrintfG(std::string Str)
{
	printf("\x1b[42m%s\x1b[m\n", Str.c_str());
}

void Console::PrintfY(std::string Str)
{
	printf("\x1b[43m%s\x1b[m\n", Str.c_str());
}
