#include "ColorPCH.h"
#include "CharUtils.h"

char TCharUtils<char>::ToLower(char Char)
{
	return (Char < 0x41 || Char > 0x5A) ? Char : Char + 0x20;
}

char TCharUtils<char>::ToUpper(char Char)
{
	return (Char < 0x61 || Char > 0x7A) ? Char : Char - 0x20;
}
