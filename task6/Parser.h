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
	SUCCESS,
	EMPTY
};


class Parser
{
private:
	string inputString;
	string resultString;
	unsigned int INDEX;
	string intValue;
	string intNumber;
	//bool caseFlag;
	//bool defaultFlag;
	Token token;

public:
	Parser(string iStr);
	~Parser();

	Token lexicalAnalyser();
	void syntaxAnalyser();

	string parseSwitch(bool caseFlag, bool defaultFlag);
	string parseAction(bool& caseFlag, bool& defaultFlag);
	string parseAction2(bool caseFlag, bool defaultFlag);

	string parsePrint(bool flag);
	string parseBreak(bool caseFlag);
	string isNumber();

	void printResult()const;
};
