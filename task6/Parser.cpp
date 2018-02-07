#include "stdafx.h"
#include "Parser.h"


Parser::Parser(string iStr) : inputString(iStr), INDEX(0)
{
	printData.clear();
	resultString.clear();
	intValue.clear();
	syntaxAnalyser();
}


string Parser::isNumber() {
	int size = inputString.length();
	int position = INDEX;
	int i = 0;
	string number = "error!";

	while ((position < size) && (isdigit(inputString[position]))) {
		i++;
		number = inputString.substr(INDEX, i);
		position++;
	}
	intValue = number;
	return number;
}


Token Parser::lexicalAnalyser()
{
	switch (inputString[INDEX])
	{
	case '(':
		return LEFT_PARENTHESIS;
		break;
	case ')':
		return RIGHT_PARENTHESIS;
		break;
	case '{':
		return LEFT_BRACE;
		break;
	case '}':
		return RIGHT_BRACE;
		break;
	case ';':
		return SEMICOLON;
		break;
	case ':':
		return DOUBLET;
		break;
	case ' ':
		return SPACE;
		break;
	default:
		if		(inputString.substr(INDEX, 6) == "switch")	return SWITCH;
		else if (inputString.substr(INDEX, 4) == "case")	return CASE;
		else if (inputString.substr(INDEX, 5) == "print")	return PRINT;
		else if (inputString.substr(INDEX, 5) == "break")	return BREAK;
		else if (inputString.substr(INDEX, 7) == "default")	return DEFAULT;
		else if (isNumber() != "error!")					return INT;
		else												return ERROR;
	}
}

void Parser::syntaxAnalyser()
{
	Token token = lexicalAnalyser();
	if (token != SWITCH) cerr << "Error: syntaxAnalyser()";
	INDEX += 6;
	resultString = parseSwitch();	
}

string Parser::parseSwitch()
{
	string intResult;
	string defaultResult;
	string caseResult;
	string switchResult;
	Token token = lexicalAnalyser();

	if (token != LEFT_PARENTHESIS)
	{
		cerr << "Error: parseSwitch() - LEFT_PARENTHESIS" << endl;
		return "Error";
	}
	INDEX++;
	intResult = parseInt();

	if (intResult != "YES")
	{
		cerr << "Error: parseInt()" << endl;
		return "Error";
	}
	token = lexicalAnalyser();
	
	if (token != RIGHT_PARENTHESIS)
	{
		cerr << "Error: parseSwitch() - RIGHT_PARENTHESIS" << endl;
		return "Error";
	}
	INDEX++;
	token = lexicalAnalyser();
	
	if (token != LEFT_BRACE)
	{
		cerr << "Error: parseSwitch() - LEFT_BRACE" << endl;
		return "Error";
	}
	INDEX++;
	token = lexicalAnalyser();

	if (token != DEFAULT) return "Error: parseSwitch() - DEFAULT";
	INDEX += 7;
	defaultResult = parseDefault();
	if (defaultResult != "YES")
	{
		cerr << "Error: parseSwitch() - parseDefault()" << endl;
		return "Error: parseSwitch() - parseDefault()";
	}
	token = lexicalAnalyser();

	cout << "INDEX is: " << INDEX << endl;
	
	if (token != RIGHT_BRACE)
	{
		cerr << "Error: parseSwitch() - RIGHT_BRACE" << endl;
		return "Error";
	}
	return "Game over! You won!!!";
}

string Parser::parseInt()
{
	string number;
	Token token = lexicalAnalyser();
	if (token != INT) return "Error: parseInt()";
	number = isNumber();
	intValue = number;
	int size = number.length();
	INDEX += size;

	if(lexicalAnalyser() == RIGHT_PARENTHESIS) return "YES";
}

string Parser::parseDefault()
{
	string printResult, breakResult;
	Token token = lexicalAnalyser();

	if (token != DOUBLET)
	{
		cerr << "Error: parseDefault() - DOUBLET" << endl;
		return "Error: parseDefault() - DOUBLET";
	}
	INDEX++;
	token = lexicalAnalyser();

	if (token != PRINT)
	{
		cerr << "Error: parseDefault() - BREAK or PRINT" << endl;
		return "Error: parseDefault() - BREAK or PRINT";
	}
	INDEX += 5;
	printResult = parsePrint();

	if (printResult != "YES")
	{
		cerr << "Error: parseDefault() - PRINT parse error"; \
		return "Error: parseDefault() - PRINT parse error";
	}
	return "YES";
}

string Parser::parseBreak()
{
	return string();
}

string Parser::parseCase()
{
	return string();
}

string Parser::parsePrint()
{
	string pInt;
	Token token = lexicalAnalyser();

	if (token != LEFT_PARENTHESIS)
	{
		cerr << "Error: parsePrint() - LEFT_PARENTHESIS" << endl;
		return "Error: parsePrint() - LEFT_PARENTHESIS";
	}
	INDEX++;
	token = lexicalAnalyser();

	/*
	if (token == SPACE)
	{
		cerr << "Error: parsePrint() - PRINT function consists SPACE"; 
		return "Error: parsePrint() - PRINT function consists SPACE";
	}
	else if (token == RIGHT_PARENTHESIS)
	{
		cerr << "Error: parsePrint() - PRINT function without data"; 
		return "Error: parsePrint() - PRINT function without data";
	}
	*/

	pInt = parseInt();
	if (pInt != "YES")
	{
		cerr << "partseInt() inside PRINT" << endl;
		return "Error";
	}
	cout << "PRINT result is: " << intValue << endl;
	token = lexicalAnalyser();

	if (token != RIGHT_PARENTHESIS)
	{
		cerr << "Error: parsePrint() - RIGHT_PARENTHESIS" << endl;
		return "Error";
	}
	INDEX++;
	token = lexicalAnalyser();

	if (token != SEMICOLON) return "Error: parsePrint() - PRINT has not finished with SEMICOLON";
	INDEX++;
	return "YES";
}


void Parser::printResult()
{	
	cout << "Input is: " << inputString << endl
		 << "Result is: " << resultString << endl;
}

Parser::~Parser()
{

}
