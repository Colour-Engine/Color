#include "HeaderToolPCH.h"
#include "CodeGen/Lexer.h"

#include "Utils/FileSystem.h"

int main(int ArgC, char* ArgV[])
{
	FFileSystem::InitFS();

	if (ArgC != 2)
	{
		printf("ColorHeaderTool is in testing stage. It only supports execution with at least one parameter provided for now. Provide the file that needs to be worked on as the only argument.\n");
		return 1;
	}

	FString FileData;
	bool bRead = FFileSystem::ReadFile(ArgV[1], FileData);

	if (!bRead)
	{
		printf("Couldn't read '%s'\n", ArgV[1]);
		return 1;
	}

	FLexer Lexer(ArgV[1], FileData);
	FToken Token;

	while ((Token = Lexer.Lexe()).Type != ETokenType::EoF)
	{
		std::cout << Token << '\n';
	}
}
