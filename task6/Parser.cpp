#include "stdafx.h"
#include "Parser.h"


Parser::Parser(string iStr) : inputString(iStr), INDEX(0), 
	intNumber("empty, no equal case value"),
	caseFlag(false)
{
	resultString.clear();
	intValue.clear();
	syntaxAnalyser();
}

inline string enumToString(Token t)
{
	switch (t)
	{
	case ERROR:
		return " syntax error ";
		break;
	case SUCCESS:
		return " successful ";
		break;
	case EMPTY:
		return " empty ";
		break;
	default:
		cerr << " Wrong token " << endl;
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
	//our grammary needs to start with switch token
	Token token = lexicalAnalyser();
	if (token != SWITCH) cerr << enumToString(ERROR);
	resultString = parseSwitch();
}

string Parser::parseSwitch()
{
	caseFlag = false;
	string actionResult = enumToString(SUCCESS);
	string action2Result;

	token = lexicalAnalyser();
	if (token != LEFT_PARENTHESIS)
	{
		//cerr << "Error: parseSwitch() - LEFT_PARENTHESIS" << endl;
		return enumToString(ERROR);
	}

	token = lexicalAnalyser();
	if (token != INT)
	{
		//cerr << "Error: token is not INT" << endl;
		return enumToString(ERROR);
	}

	intNumber = intValue;

	token = lexicalAnalyser();
	if (token != RIGHT_PARENTHESIS)
	{
		//cerr << "Error: parseSwitch() - RIGHT_PARENTHESIS" << endl;
		return enumToString(ERROR);
	}

	token = lexicalAnalyser();
	if (token != LEFT_BRACE)
	{
		//cerr << "Error: parseSwitch() - LEFT_BRACE" << endl;
		return enumToString(ERROR);
	}

	token = lexicalAnalyser();
	if (token == CASE)
	{
		actionResult = parseAction();
		//token = lexicalAnalyser();
		if (actionResult != enumToString(SUCCESS))
		{
			//cerr << "Error: parseSwitch() - parseAction()" << endl;
			return enumToString(ERROR);
		}
	}

	if (token != DEFAULT)
	{
		//cerr << "Error: parseSwitch() - DEFAULT" << endl;
		return enumToString(ERROR);
	}

	token = lexicalAnalyser();
	if (token != DOUBLET)
	{
		//cerr << "Error: parseDefault() - DOUBLET" << endl;
		return enumToString(ERROR);
	}

	
	if (caseFlag == false)
	{
		token = lexicalAnalyser();
		action2Result = parseAction2();
	}
	else if (caseFlag == true) action2Result = enumToString(SUCCESS);

	if (action2Result != enumToString(SUCCESS))
	{
		//cerr << "Error: action2Result failed in SWITCH" << endl;
		return enumToString(ERROR);
	}

	token = lexicalAnalyser();
	if (token != RIGHT_BRACE)
	{
		//cerr << "Error: parseSwitch() - RIGHT_BRACE" << endl;
		return enumToString(ERROR);
	}

	//token = EMPTY;

	return enumToString(SUCCESS);
}

string Parser::parseAction()
{
	string action2Result;
	token = lexicalAnalyser();
	if (token != INT)
	{
		//cerr << "Error: parseAction() - INT" << endl;
		return enumToString(ERROR);
	}

	if (intNumber == intValue)	caseFlag = true;
	else
	{
		while (token != ERROR)
		{
			token = lexicalAnalyser();
			if ((token == CASE) || (token == DEFAULT)) break;
			else continue;
		} 

		if (token == CASE) parseAction();
		else if (token == DEFAULT)
		{
			caseFlag = false;
			return enumToString(SUCCESS);
		}
		else return enumToString(ERROR);
	}

	token = lexicalAnalyser();
	if (token != DOUBLET)
	{
		//cerr << "Error: parseAction() - DOUBLET" << endl;
		return enumToString(ERROR);
	}

	token = lexicalAnalyser();
	if (token == CASE)  parseAction();
	else if (token == SWITCH
		|| token == BREAK
		|| token == PRINT) action2Result = parseAction2();

	if (action2Result != enumToString(SUCCESS))
	{
		//cerr << "Error: parseAction2() inside CASE failed " << endl;
		return enumToString(ERROR);
	}

	return enumToString(SUCCESS);
}

string Parser::parseAction2()
{
	string printResult, breakResult, switchResult, action2Result;

	if (token == PRINT)
	{
		printResult = parsePrint();
		if (printResult != enumToString(SUCCESS))
		{
			//cerr << "Error: parse PRINT inside parseAction2()" << endl;
			return enumToString(ERROR);
		}

		token = lexicalAnalyser();

		if (token == SWITCH || token == PRINT || token == BREAK)
		{
			action2Result = parseAction2();
			if (action2Result != enumToString(SUCCESS))
			{
				//cerr << "Error: parseAction2() inside parseAction2()" << endl;
				return enumToString(ERROR);
			}
		}
		else if (token == RIGHT_BRACE) INDEX--;
		else return enumToString(ERROR);
	}

	else if (token == BREAK)
	{
		breakResult = parseBreak();
		if (breakResult != enumToString(SUCCESS))
		{
			//cerr << "Error: parse BREAK inside parseAction2()" << endl;
			return enumToString(ERROR);
		}
	}

	else if (token == SWITCH)
	{
		switchResult = parseSwitch();
		if (switchResult != enumToString(SUCCESS))
		{
			//cerr << "Error: parse SWITCH inside parseAction2()" << endl;
			return enumToString(ERROR);
		}
		token = lexicalAnalyser();
		caseFlag = true;
	}

	return enumToString(SUCCESS);
}

string Parser::parseBreak()
{
	if (caseFlag == true)
	{
		printResult();
		exit(0);
	}

	token = lexicalAnalyser();
	if (token != SEMICOLON)
	{
		//cerr << "Error: parseBreak()" << endl;
		return enumToString(ERROR);
	}

	return enumToString(SUCCESS);
}

string Parser::parsePrint()
{
	token = lexicalAnalyser();
	if (token != LEFT_PARENTHESIS)
	{
		//cerr << "Error: parsePrint() - LEFT_PARENTHESIS" << endl;
		return enumToString(ERROR);
	}

	token = lexicalAnalyser();
	cout << "PRINT result: " << intValue << endl;
	if (token != INT)
	{
		//cerr << "INT error inside PRINT" << endl;
		return enumToString(ERROR);
	}

	token = lexicalAnalyser();
	if (token != RIGHT_PARENTHESIS)
	{
		cerr << "Error: parsePrint() - RIGHT_PARENTHESIS" << endl;
		return enumToString(ERROR);
	}

	token = lexicalAnalyser();
	if (token != SEMICOLON) return "Error: parsePrint() - PRINT has not finished with SEMICOLON";

	return enumToString(SUCCESS);
}


void Parser::printResult() const
{
	cout << "\nInput is: "  << inputString << endl
		 << "Result is: " << resultString << endl;
}

Parser::~Parser()
{

}
