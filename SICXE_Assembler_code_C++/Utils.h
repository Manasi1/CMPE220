#ifndef UTILS_H_
#define UTILS_H_

#include <algorithm>
#include <bitset>
#include <fstream>
#include <iostream>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>
#include <queue>
#include "sstream"
#include "DataStructures.h"

namespace StringUtil
{
	string convertIntegerToString(int integer, bool hex);

	void adjustStringLength(string* line, int length, char filling, bool lhs);

	string convertToUpperCase(string str);

	string trimSpaces(string str);

	bool ifNumeric(string s);

	bool ifHexadecimal(string s);

	int parsingDecimalFromString(string dec);

	int parsingHexadecimalFromString(string hexd);

	float parsingFloatFromString(string flt);

	vector<string> splitDelimiter(string x, char s);

	string mergingStrings(string x, string y, char del);

	vector<string> generalizedSplitter(string toSplit, string splitters);

	parsedLines parsingLines(string line);
}

namespace ExpressionUtil
{
	bool validatingExpression(string expression);

	bool ifRelativeExpression(string expression);

	int evaluatingExpression(string expression, currSect* cs, bool ifTestValueUsed = false);
}

namespace OperandUtil
{
	int getLiteralOperandValue(string op);
	int operandEvaluation(string operand, currSect* csect = NULL);
	bool ifValidOperand(string operand);
	int findTypeOfExpression(string op);
	int getLiteralOperandAddress(string op);
}

namespace ValidateUtil
{
	int validation(parsedLines pl);
	bool checkIfGarbageValue(string x);
	bool checkCharacter(char x);
	bool checkIfNumber(char a);
	bool ifReg(string o);
	int formatOne(string o1, string o2);
	int formatTwo(string op, string o1, string o2);
	int format34(string op, string o1, string o2);
	void insertDirectiveValues();
	bool checkByteValue(string x);
	bool checkLiteralValue(string x, bool isReg);
	bool checkIfExt(string s);
}
#endif
