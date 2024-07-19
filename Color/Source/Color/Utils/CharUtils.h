#pragma once

template <typename T>
struct TCharUtils
{
};

template <>
struct TCharUtils<char>
{
	static char ToLower(char Char);
	static char ToUpper(char Char);
};
