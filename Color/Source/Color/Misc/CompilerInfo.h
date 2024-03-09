#pragma once

enum class ECompiler
{
	MSVC,
	Clang,
	GCC
};

struct FCompiler
{
	const char* Name;
	const char* Abbreviation;
	ECompiler Compiler;
};

#ifdef _MSC_VER
	#define CL_COMPILER_MSVC 1
	inline constexpr FCompiler CCompiler =
	{
		"Microsoft Visual C++ Compiler",
		"MSVC",
		 ECompiler::MSVC
	};
#elif defined __clang__
	#define CL_COMPILER_CLANG 1
	inline constexpr FCompiler CCompiler =
	{
		"Clang++",
		"Clang",
		 ECompiler::Clang
	};
#elif defined __GNUC__
	#define CL_COMPILER_GCC 1
	inline constexpr FCompiler CCompiler =
	{
		"GNU C++ Compiler",
		"GCC",
		 ECompiler::GCC
	};
#else
	#error Unknown compiler!
#endif
