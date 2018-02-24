#include "stdafx.h"
#include "Parser.h"


Parser::Parser(string iStr) : inputString(iStr), INDEX(0)
{
	printData.clear();
	resultString.clear();
	intValue.clear();
	syntaxAnalyser();
}


inline string enumToString(Token t)
{
	switch (t)
	{
	case ERROR:
		return "syntax error";
		break;
	case SUCCESS:
		return "successful";
		break;
	}
}

string Parser::isNumber() {
	int size = inputString.length();
	int position = INDEX;
	int i = 0;
	string number = enumToString(ERROR);

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
		INDEX++;
		return LEFT_PARENTHESIS;
		break;
	case ')':
		INDEX++;
		return RIGHT_PARENTHESIS;
		break;
	case '{':
		INDEX++;
		return LEFT_BRACE;
		break;
	case '}':
		INDEX++;
		return RIGHT_BRACE;
		break;
	case ';':
		INDEX++;
		return SEMICOLON;
		break;
	case ':':
		INDEX++;
		return DOUBLET;
		break;
	case ' ':
		INDEX++;
		return SPACE;
		break;
	default:
		     if (inputString.substr(INDEX, 6) == "switch")  { INDEX += 6; return SWITCH;  }
		else if (inputString.substr(INDEX, 4) == "case")    { INDEX += 4; return CASE;    }
		else if (inputString.substr(INDEX, 5) == "print")   { INDEX += 5; return PRINT;   }
		else if (inputString.substr(INDEX, 5) == "break")   { INDEX += 5; return BREAK;   }
		else if (inputString.substr(INDEX, 7) == "default") { INDEX += 7; return DEFAULT; }
		else if (isNumber() != enumToString(ERROR))
		{
			intValue = isNumber();
			INDEX += intValue.length();
			return INT;
		}
		else return ERROR;
	}
}

void Parser::syntaxAnalyser()
{
	Token token = lexicalAnalyser();
	if (token != SWITCH) cerr << "Error: syntaxAnalyser()";
	//INDEX += 6;
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
		return enumToString(ERROR);
	}
	
	//INDEX++;

	/*
	intResult = parseInt();
	if (intResult != enumToString(SUCCESS))
	{
		cerr << "Error: parseInt()" << endl;
		return enumToString(ERROR);
	}
	*/

	token = lexicalAnalyser();
	//cout << "INT: " << intValue << endl;
	if (token != INT)
	{
		cerr << "Error: parseInt()" << endl;
		return enumToString(ERROR);
	}

	token = lexicalAnalyser();	
	if (token != RIGHT_PARENTHESIS)
	{
		cerr << "Error: parseSwitch() - RIGHT_PARENTHESIS" << endl;
		return enumToString(ERROR);
	}
	
	//INDEX++;
	
	token = lexicalAnalyser();
	if (token != LEFT_BRACE)
	{
		cerr << "Error: parseSwitch() - LEFT_BRACE" << endl;
		return enumToString(ERROR);
	}
	
	//INDEX++;
	
	token = lexicalAnalyser();
	if (token != DEFAULT)
	{
		cerr << "Error: parseSwitch() - DEFAULT" << endl;
		return enumToString(ERROR);
	}

	//INDEX += 7;

	defaultResult = parseDefault();
	if (defaultResult != enumToString(SUCCESS))
	{
		cerr << "Error: parseSwitch() - parseDefault()" << endl;
		return enumToString(ERROR);
	}
	
	token = lexicalAnalyser();
	cout << "INDEX is: " << INDEX << endl;	
	if (token != RIGHT_BRACE)
	{
		cerr << "Error: parseSwitch() - RIGHT_BRACE" << endl;
		return enumToString(ERROR);
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
		return enumToString(ERROR);
	}

	//INDEX++;

	token = lexicalAnalyser();
	if (token != PRINT)
	{
		cerr << "Error: parseDefault() - BREAK or PRINT" << endl;
		return enumToString(ERROR);
	}

	//INDEX += 5;

	printResult = parsePrint();
	if (printResult != enumToString(SUCCESS))
	{
		cerr << "Error: parseDefault() - PRINT parse error";
		return enumToString(ERROR);
	}

	return enumToString(SUCCESS);
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
		return enumToString(ERROR);
	}

	//INDEX++;
	//pInt = parseInt();

	token = lexicalAnalyser();
	cout << "INT inside PRINT: " << intValue << endl;
	if (token != INT)
	{
		cerr << "INT error inside PRINT" << endl;
		return enumToString(ERROR);
	}
/*
	if (pInt != "YES")
	{
		cerr << "partseInt() inside PRINT" << endl;
		return enumToString(ERROR);
	}
	*/
	//cout << "PRINT result is: " << intValue << endl;

	token = lexicalAnalyser();
	if (token != RIGHT_PARENTHESIS)
	{
		cerr << "Error: parsePrint() - RIGHT_PARENTHESIS" << endl;
		return enumToString(ERROR);
	}

	//INDEX++;

	token = lexicalAnalyser();
	if (token != SEMICOLON) return "Error: parsePrint() - PRINT has not finished with SEMICOLON";

	//INDEX++;
	return enumToString(SUCCESS);
}


void Parser::printResult()
{	
	cout << "Input is: "  << inputString  << endl
		 << "Result is: " << resultString << endl;
}

Parser::~Parser()
{

}
