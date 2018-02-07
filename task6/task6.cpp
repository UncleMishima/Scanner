// task6.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Parser.h"

int main()
{
	string input;
	getline(cin, input);
	Parser* parser = new Parser(input);
	parser->printResult();

	system("pause");
    return 0;
}

