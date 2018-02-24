#pragma once

#include <string>
#include <iostream>
#include <cstdlib>
#include <cctype>
#include <ctype.h>
#include <locale>

using namespace std;

enum Token
{
	SWITCH,
	LEFT_PARENTHESIS,
	RIGHT_PARENTHESIS,
	INT,
	LEFT_BRACE,
	RIGHT_BRACE,
	CASE,
	PRINT,
	BREAK,
	DEFAULT,
	SPACE,
	SEMICOLON,
	DOUBLET,
	ERROR,
	SUCCESS
};


class Parser
{
private:
	string inputString;
	string resultString;
	unsigned int INDEX;
	string intValue;
	string printData;
	string intNumber;
	Token token;

public:
	Parser(string iStr);
	~Parser();

	Token lexicalAnalyser();
	bool checkToken(string str);
	void syntaxAnalyser();

	string parseSwitch();

	string parseAction();
	string parseAction2();
	string parsePrint();

	string parseDefault();
	string parseBreak();
	
	string isNumber();


	void printResult();
};
