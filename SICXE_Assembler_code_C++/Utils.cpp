#include "Utils.h"
#include "OpcodeTable.h"

namespace StringUtil
{
	string convertIntegerToString(int integer, bool hex) {
		stringstream ss;
		string intAsStr = "";
		if (hex)
			ss << uppercase << std::hex << integer;
		else
			ss << integer;
		intAsStr = ss.str();
		return intAsStr;
	}

	void adjustStringLength(string* line, int length, char filling, bool lhs) {
		if (lhs)
			for (int i = line->length(); i < length; i++)
				*line = filling + *line;
		else
			for (int i = line->length(); i < length; i++)
				*line += filling;
	}

	string convertToUpperCase(string str) {
		unsigned int i;
		string toRet = "";
		for (i = 0; i < str.length(); i++) {
			char pos = str.at(i);
			if (pos <= 122 && pos >= 97) {
				pos -= 32;
			}
			toRet += pos;
		}
		return toRet;
	}

	/*
	* Trims the leading and forwarding spaces.
	*/
	string trimSpaces(string str) {

		if (str.compare("") == 0)
			return str;

		unsigned int i;
		for (i = 0; i < str.length() && str.at(i) == ' '; i++)
			;

		if (i == str.length())
			return "";

		unsigned int j;
		for (j = str.length() - 1; j >= 0 && str.at(j) == ' '; j--)
			;

		string toRet = "";
		unsigned int k;
		for (k = i; k <= j; k++)
			toRet += str.at(k);

		return toRet;
	}

	/*
	* Parses a decimal integer from a string.
	*/
	int parsingDecimalFromString(string dec) {
		char first = dec.at(0);
		unsigned int i = first == '-' || first == '+' ? 1 : 0;

		int result = 0;
		for (; i < dec.length(); i++) {
			result *= 10;
			result += (dec.at(i) - 48);
		}
		return first == '-' ? -result : result;
	}

	/*
	* Parses a hexadecimal integer from a string.
	*/

	int parsingHexadecimalFromString(string hexd) {
		char first = hexd.at(0);
		unsigned int i = first == '-' || first == '+' ? 1 : 0;

		int result = 0;
		for (; i < hexd.length(); i++) {
			result *= 16;

			if (hexd.at(i) >= 48 && hexd.at(i) <= 57)
				result += (hexd.at(i) - 48);
			else
				result += (hexd.at(i) - 55);
		}
		return first == '-' ? -result : result;
	}

	/*
	* Parses a float from a string.
	*/
	float parsingFloatFromString(string flt) {
		float resFrac = 0.0, resInt = 0.0;
		unsigned int i;

		unsigned int dotPos = flt.find(".");

		if (dotPos != flt.npos) {
			for (i = dotPos + 1; i < flt.length(); i++) {
				resFrac += (flt.at(i) - 48);
				resFrac *= (float)0.1;
			}
		}

		char first = flt.at(0);
		i = first == '-' || first == '+' ? 1 : 0;

		for (; i <= dotPos - 1; i++) {
			resInt *= 10;
			resInt += (flt.at(i) - 48);
		}
		float res = resFrac;
		res += resInt;

		return first == '-' ? -res : res;
	}

	/*
	* Splits the given string using the character s as a delimiter.
	*/
	vector<string> splitDelimiter(string x, char s) {
		vector<string> toRet;
		string temp = "";
		for (unsigned int i = 0; i < x.length(); i++) {
			if (x.at(i) == s) {
				if (temp.length() == 0)
					continue;

				toRet.push_back(temp);
				temp = "";
				continue;
			}
			(temp) += x.at(i);
		}
		if (temp.length() != 0)
			toRet.push_back(temp);

		return toRet;
	}

	string mergingStrings(string x, string y, char del) {
		x = trimSpaces(x);
		y = trimSpaces(y);

		if (y.compare("") != 0) {
			return x.append("" + del).append(y);
		}
		return x;
	}

	bool ifNumeric(string s) {
		if (s.length() == 0)
			return false;
		for (unsigned int i = 0; i < s.length(); i++) {
			if (s.at(i) - '0' < 0 || s.at(i) - '0' > 9)
				return false;
		}
		return true;
	}

	bool ifHexadecimal(string s) {
		if (s.length() == 0)
			return false;
		for (unsigned int i = 0; i < s.length(); i++) {
			if (s.at(i) != 'A' && s.at(i) != 'B' && s.at(i) != 'C' && s.at(i) != 'D'
				&& s.at(i) != 'E' && s.at(i) != 'F'
				&& (s.at(i) - '0' < 0 || s.at(i) - '0' > 9))
				return false;
		}
		return true;
	}

	vector<string> generalizedSplitter(string toSplit, string splitters) {
		vector<string> toRet;
		string temp = "";
		for (unsigned int i = 0; i < toSplit.length(); i++) {
			if (splitters.find(toSplit.at(i)) != string::npos) {
				if (temp.length() == 0)
					continue;

				toRet.push_back(temp);
				temp = "";
				continue;
			}
			(temp) += toSplit.at(i);
		}
		if (temp.length() != 0)
			toRet.push_back(temp);

		return toRet;
	}

	bool checkForOperands(string s) {
		//This function check the operand for Format 1 type.
		//Because in that case no operand will be required to be supplies with them.
		s = convertToUpperCase(s);
		//Whole string is converted to Uppercase to bring uniformity for comparision
		return s.compare("RSUB") == 0 || s.compare("NOBASE") == 0
			|| s.compare("HIO") == 0 || s.compare("FIX") == 0
			|| s.compare("NORM") == 0 || s.compare("TIO") == 0 || s.compare("START") == 0
			|| s.compare("SIO") == 0 || s.compare("FLOAT") == 0 || s.compare("CSECT") == 0;
	}

	parsedLines parsingLines(string line) {
		//This function read the line supplied as argument and split it into
		// fundamental elements of an instruction like lable, opcode, operand1 and
		//operand2 if they exists.
		unsigned int i;
		parsedLines pl;
		pl.labels = "";
		pl.opCodes = "";
		pl.operand1 = "";
		pl.operand2 = "";

		string s[5] = { "", "", "", "", "" };
		//Initialising the array with empty string which will be replaced as string
		//is parsed
		i = 0;
		for (unsigned int j = 0; j < 5 && i < line.length(); j++) {
			for (; i < line.length() && (line[i] == ' ' || line[i] == '\t'); i++);
			// Skiping all blank spaces and tabs until it encounter characters
			// So that freely formated SIC/XE instruction can also be converted by
			// assembler
			for (; i < line.length() && (line[i] != ' ' && line[i] != '\t'); i++)
				s[j] += line[i];
			//When some character is encountered words are being added in the
			// respective location in s[]

			if (j == 0) {
				string temp = convertToUpperCase(s[0]);
				if (temp.compare("EXTREF") == 0 || temp.compare("EXTDEF") == 0) {
					pl.opCodes = s[0];
					for (uint32_t k = i; k<line.length(); k++)
						//if(line[k]!=' ' && line[k]!='\t')
						pl.operand1 += line[k];
					return pl;
				}
			}
			else if (j == 1) {
				string temp = convertToUpperCase(s[1]);
				if (temp.compare("EXTREF") == 0 || temp.compare("EXTDEF") == 0) {
					pl.labels = s[0];
					pl.opCodes = s[1];
					for (uint32_t k = i; k<line.length(); k++)
						//if(line[k]!=' ' && line[k]!='\t')
						pl.operand1 += line[k];
					return pl;
				}
			}

		}
		for (; i < line.length() && (line[i] == ' ' || line[i] == '\t'); i++);

		if (s[0] != "" && s[0][0] == '.') {        // comment line
			for (i = 0; i < line.length() && (line[i] == ' ' || line[i] == '\t'); i++);
			for (; i < line.length(); i++)
				pl.opCodes += line[i];
			return pl;
		}

		if (i != line.length())
			throw new string("Too Many Arguments");
		// line containging too many arguments


		if (s[0] == "")
			return pl;
		else if (s[1] == "")
			pl.opCodes = s[0];
		else if (s[2] == "") {
			if (checkForOperands(s[1])) {
				pl.labels = s[0];
				pl.opCodes = s[1];
			}
			else {
				pl.opCodes = s[0];
				pl.operand1 = s[1];
			}
		}
		else if (s[3] == "") {
			if (s[1][s[1].length() - 1] == ',' || s[2][0] == ',') {
				pl.opCodes = s[0];
				pl.operand1 = s[1] + s[2];
			}
			else {
				pl.labels = s[0];
				pl.opCodes = s[1];
				pl.operand1 = s[2];
			}
		}
		else if (s[4] == "") {
			if (s[2].compare(",") == 0) {
				pl.opCodes = s[0];
				pl.operand1 = s[1] + s[2] + s[3];
			}
			else {
				pl.labels = s[0];
				pl.opCodes = s[1];
				pl.operand1 = s[2] + s[3];
			}
		}
		else {
			if (s[3].compare(",") != 0)
				throw new string("Too Many Arguments");
			// line containging too many arguments

			pl.labels = s[0];
			pl.opCodes = s[1];
			pl.operand1 = s[2] + s[3] + s[4];
		}

		s[0] = "";
		s[1] = "";
		for (i = 0; i < pl.operand1.length() && pl.operand1[i] != ','; i++)
			s[0] += pl.operand1[i];
		if (i == pl.operand1.length() - 1 && pl.operand1[i] == ',')
			s[1] = ",";
		else
			for (i++; i < pl.operand1.length(); i++)
				s[1] += pl.operand1[i];
		pl.operand1 = s[0];
		pl.operand2 = s[1];
		return pl;
	}

}

namespace ExpressionUtil
{
	currSect* g_csect = NULL;

	void setCSECT(currSect* x) {
		g_csect = x;
	}

	bool toReplace(std::string& str, const std::string& from, const std::string& to) {
		size_t start_pos = str.find(from);
		if (start_pos == std::string::npos)
			return false;
		str.replace(start_pos, from.length(), to);
		return true;
	}

	void replacingAll(std::string& str, const std::string& from,
                      const std::string& to) {
		if (from.empty())
			return;
		size_t start_pos = 0;
		while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
			str.replace(start_pos, from.length(), to);
			start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
		}
	}

	bool ifValidNumber(string num) {
		unsigned int i = 0;
		if (num[0] == '-')
			i++;
		for (; i < num.length(); i++)
			if (num[i] < '0' || num[i] > '9')
				return false;
		return true;
	}

	bool ifValidOperandName(string operand) {
		for (unsigned int i = 0; i < operand.length(); i++)
			if ((operand[i] < 'A' || operand[i] > 'Z')
				&& (operand[i] < 'a' || operand[i] > 'z'))
				return false;
		return true;
	}

	bool ifOperator(char c) {
		return c == '+' || c == '/' || c == '*' || c == '-' || c == '%' || c == '('
			|| c == ')';
	}

	string valueFromOptab(string name, bool useDummyValues = false) { // get value from optab
		if (name[0] >= '0' && name[0] <= '9')
			return name;
		if (useDummyValues)
			return "10";

		if (name.compare("*") == 0) {
			return StringUtil::convertIntegerToString(g_csect->locCtr, false);
		}

		try {
			return StringUtil::convertIntegerToString(g_csect->symTab.at(name)->address, false);
		}
		catch (out_of_range& /*orr*/) {
			if (find(g_csect->extRef.begin(), g_csect->extRef.end(), name)
				!= g_csect->extRef.end()) {
				return "0";
			}
			else {
				throw(string) "label not found";
			}
		}
	}

	string substitution(string expression, bool useDummyValues = false) {
		string exp = ""; //.replaceAll(" ", "");   // assuming no spaces in expression
		string name = "";
		bool nextCouldBeAsterix = true;

		for (unsigned int i = 0; i < expression.length();) {
			name = "";

			while (i < expression.length() && !ifOperator(expression[i]))
				name += expression[i++];

			if (name.length() == 0 && nextCouldBeAsterix && expression[i] == '*')
				name += expression[i++];

			if (name.compare("") != 0)
				exp += valueFromOptab(name, useDummyValues); // should be replaced with value from optab

			nextCouldBeAsterix = false;
			while (i < expression.length() && ifOperator(expression[i])) {
				if (nextCouldBeAsterix && expression[i] == '*')
					exp += valueFromOptab("*", useDummyValues);
				else
					exp += expression[i];

				if (expression[i] != ')' && (!nextCouldBeAsterix || expression[i] != '*'))
					nextCouldBeAsterix = true;
				else
					nextCouldBeAsterix = false;
				i++;
			}
		}
		return exp;
	}

	int priorityChecker(char opr1, char opr2) {
		if (opr1 == opr2)
			return 0;
		if (opr1 == ')')
			return -1;
		if (opr2 == ')')
			return 1;
		if (opr1 == '*' || opr1 == '/' || opr1 == '%')
			return 1;
		if (opr2 == '*' || opr2 == '/' || opr2 == '%')
			return -1;
		if (opr1 == '+' || opr1 == '-')
			return 1;
		if (opr2 == '+' || opr2 == '-')
			return -1;
		if (opr2 == '(')
			return -1;
		if (opr1 == '(')
			return 1;
		return 0;
	}

	float simpleExpressionEvaluator(int op1, char operation, int op2) {
		switch (operation) {
		case '*':
			return (float)(op1 * op2);
		case '/':
			return (float)(op1 / op2);
		case '+':
			return (float)(op1 + op2);
		case '-':
			return (float)(op1 - op2);
		case '%':
			return (float)(op1 % op2);
		}
		return false;
	}

	int relativeDegreeResult(int relDeg1, int val1, char operation,
                             int relDeg2, int val2) {
		switch (operation) {
		case '*':
			if (relDeg1 != 0 && relDeg2 != 0)
				throw(string) "Error multiplying relative addresses";
			if (relDeg1 == 0 && relDeg2 == 0)
				return 0;
			return (relDeg1 != 0) ? relDeg1 * val2 : relDeg2 * val1;
		case '/':
			if (relDeg1 != 0 || relDeg2 != 0)
				throw(string) "Error deviding relative addresses";
			return 0;
		case '+':
			return relDeg1 + relDeg2;
		case '-':
			return relDeg1 - relDeg2;
		case '%':
			if (relDeg1 != 0 || relDeg2 != 0)
				throw(string) "Error taking modulus of relative addresses";
			return 0;
		}
		return 0;
	}

	int explicitExpressionValue(string exp, queue<char> isRelative,
                                bool *retFlagIsRelative) {
		stack<char> operations;
		stack<int> values, degreeOfRelativity;

		string op = "";
		unsigned int i;
		bool lastAddedIsOperand = false;
		for (i = 0; i < exp.length();) {
			op = "";
			while ((i < exp.length() && !ifOperator(exp[i]))
				|| (exp[i] == '-' && op.length() == 0 && !lastAddedIsOperand))
				op += exp[i++];
			if (op.compare("-") == 0) {
				values.push(-1);
				degreeOfRelativity.push(0);
				operations.push('*');
				lastAddedIsOperand = false;

			}
			else if (op.length() != 0) {
				values.push(atoi(op.c_str()));
				degreeOfRelativity.push((isRelative.front() == 'R') ? 1 : 0);
				isRelative.pop();
				lastAddedIsOperand = true;
			}

			else if (operations.empty() || exp[i] == '('
                     || priorityChecker(operations.top(), exp[i]) != 1) {
				operations.push(exp[i++]);
				lastAddedIsOperand = false;
			}
			else // evaluate the top operation in stack
			{
				if (operations.top() == '(' && exp[i] == ')') {
					operations.pop();
					i++;
				}
				else {
					int op2 = values.top();
					values.pop();
					int rd2 = degreeOfRelativity.top();
					degreeOfRelativity.pop();
					int op1 = values.top();
					values.pop();
					int rd1 = degreeOfRelativity.top();
					degreeOfRelativity.pop();
					float result = simpleExpressionEvaluator(op1, operations.top(), op2);
					degreeOfRelativity.push(
                            relativeDegreeResult(rd1, op1, operations.top(),
                                                 rd2, op2));
					operations.pop();
					values.push(int(result));
				}
			}
		}
		while (operations.size() != 0) {
			int op2 = values.top();
			values.pop();
			int rd2 = degreeOfRelativity.top();
			degreeOfRelativity.pop();
			int op1 = values.top();
			values.pop();
			int rd1 = degreeOfRelativity.top();
			degreeOfRelativity.pop();
			float result = simpleExpressionEvaluator(op1, operations.top(), op2);
			degreeOfRelativity.push(
                    relativeDegreeResult(rd1, op1, operations.top(), rd2,
                                         op2));
			operations.pop();
			values.push(int(result));
		}

		if (values.size() == 1) {
			if (degreeOfRelativity.top() != 1 && degreeOfRelativity.top() != 0)
				throw(string) "invalid operations on relative variables";
			*retFlagIsRelative = (degreeOfRelativity.top() == 0) ? false : true;
			int ret = values.top();
			values.pop();
			return ret;
		}
		// through exception
		throw(string) "error evaluating expression";
	}

	queue<char> expressionValidator(string expression) { // returns queue as map for values whether are relative or absolute
		queue<char> exp;
		string operand = "";
		bool nextSouldBeOperand = true;
		int bracketCounter = 0;

		for (unsigned int i = 0; i < expression.length();) {
			operand = "";
			while (i < expression.length() && !ifOperator(expression[i]))
				operand += expression[i++];

			if (operand.length() > 0) {
				if (!nextSouldBeOperand)
					throw(string) "Invalid operations in expression1";
				if (ifValidNumber(operand))
					exp.push('A');
				else if (ifValidOperandName(operand))
					exp.push('R');
				else
					throw(string) "Bad Operand Name";
				nextSouldBeOperand = false;
			}

			while (i < expression.length() && ifOperator(expression[i])) {
				if (nextSouldBeOperand && expression[i] == '-')
					break;
				if (nextSouldBeOperand) {
					if (expression[i] == '*') {
						exp.push('R');
						nextSouldBeOperand = false;
					}
					else if (expression[i] == '(')
						nextSouldBeOperand = true;
					else
						throw(string) "Invalid operations in expression";
				}
				else {
					if (expression[i] != ')')
						nextSouldBeOperand = true;
					else
						nextSouldBeOperand = false;
				}
				if (expression[i] == '(')
					bracketCounter++;
				if (expression[i] == ')')
					bracketCounter--;
				if (bracketCounter < 0)
					throw(string) "non-matching brackets";
				i++;
			}
		}
		if (nextSouldBeOperand)
			throw(string) "missing operand";
		if (bracketCounter != 0)
			throw(string) "non-matching brackets";

		return exp;
	}

	int evaluatingExpression(string expression, currSect* cs, bool ifTestValueUsed) {
		bool x;
		setCSECT(cs);
		return explicitExpressionValue(substitution(expression, ifTestValueUsed), expressionValidator(expression), &x);
	}

	bool validatingExpression(string expression) {
		try {
            evaluatingExpression(expression, g_csect, true);
		}
		catch (string& /*s*/) {
			return false;
		}
		return true;
	}

	bool ifRelativeExpression(string expression) {
		bool x;
        explicitExpressionValue(substitution(expression, true), expressionValidator(expression), &x);
		return x;
	}
}

namespace OperandUtil
{
	currSect* g_curCSECT;

	void setCurrentCSECTValue(currSect* x) {
		g_curCSECT = x;
	}

	int gettingValueFromOptab(string name) { // get value from optab
		if (name.compare("*") == 0) {
			return g_curCSECT->locCtr;
		}

		try {
			return g_curCSECT->symTab.at(name)->address;
		}
		catch (out_of_range& /*orr*/) {
			if (find(g_curCSECT->extRef.begin(), g_curCSECT->extRef.end(), name)
				!= g_curCSECT->extRef.end()) {
				return 0;
			}
			else {
				throw(string) "label not found";
			}
		}
	}

	bool ifValidNumber(string num) {
		unsigned int i = 0;
		if (num[0] == '-')
			i++;
		for (; i < num.length(); i++)
			if (num[i] < '0' || num[i] > '9')
				return false;
		return true;
	}

	bool ifValidOperand(string operand) {
		if (operand.compare("*") == 0)
			return true;
		for (unsigned int i = 0; i < operand.length(); i++)
			if ((operand[i] < 'A' || operand[i] > 'Z')
				&& (operand[i] < 'a' || operand[i] > 'z'))
				return false;
		return true;
	}

	bool ifValidHexadecimalConstant(string op) {
		if (op.length() < 3 || op[0] != 'X' || op[1] != '\''
			|| op[op.length() - 1] != '\'')
			return false;
		return StringUtil::ifHexadecimal(op.substr(2, op.length() - 3));
	}

	bool ifValidByteConstant(string op) {
		if (op.length() < 3 || op[0] != 'C' || op[1] != '\''
			|| op[op.length() - 1] != '\'')
			return false;
		return true;
	}

	bool ifValidImmediateOperand(string op) {
		if (op.length() < 2 || op[0] != '#')
			return false;
		string rest = op.substr(1);
		return ifValidNumber(rest) || ifValidOperand(rest) || ifValidHexadecimalConstant(rest)
               || ifValidByteConstant(rest);
	}

	bool ifValidIndirectOperand(string op) {
		if (op.length() < 2 || op[0] != '@')
			return false;
		string rest = op.substr(1);
		return ifValidNumber(rest) || ifValidOperand(rest);
	}

	bool ifValidLiteralOperand(string op) {
		if (op.length() < 2 || op[0] != '=')
			return false;
		string rest = op.substr(1);
		//////// don't know what to write
		return ifValidNumber(rest) || ifValidOperand(rest) || ifValidHexadecimalConstant(rest)
               || ifValidByteConstant(rest);
	}

	int findTypeOfExpression(string op) {
		if (op.length() == 0)
			return -1;
		if (ifValidImmediateOperand(op))
			return 0;
		if (ifValidIndirectOperand(op))
			return 1;
		if (ifValidLiteralOperand(op))
			return 2;
		if (ifValidHexadecimalConstant(op))
			return 3;
		if (ifValidByteConstant(op))
			return 4;
		if (ifValidNumber(op))
			return 5;
		if (ExpressionUtil::validatingExpression(op))
			return 6;
		return -1;
	}

	int getByteConstantValue(string op) {
		int value = 0;
		for (unsigned int i = 0; i < op.length(); i++)
			value += ((int)op[i]) << (i * 8);
		return value;
	}

	int getImmediateOperandValue(string op) {
		string rest = op.substr(1);
		if (ifValidHexadecimalConstant(rest))
			return StringUtil::parsingHexadecimalFromString(rest.substr(2, rest.length() - 3));
		if (ifValidByteConstant(rest))
			return getByteConstantValue(rest.substr(2, rest.length() - 3));
		if (ifValidOperand(rest))
			return gettingValueFromOptab(rest);
		if (ifValidNumber(rest))
			return StringUtil::parsingDecimalFromString(rest);

		throw(string) "invalid immediate operand";
	}

	int getIndirectOperandValue(string op) {
		string rest = op.substr(1);
		if (ifValidOperand(rest))
			return gettingValueFromOptab(rest);
		if (ifValidNumber(rest))
			return StringUtil::parsingDecimalFromString(rest);
		throw(string) "invalid indirect operand";
	}

	int getLiteralOperandValue(string op) {

		string rest = op.substr(1);
		if (ifValidHexadecimalConstant(rest))
			return StringUtil::parsingHexadecimalFromString(rest.substr(2, rest.length() - 3));
		if (ifValidByteConstant(rest))
			return getByteConstantValue(rest.substr(2, rest.length() - 3));
		if (ifValidNumber(rest))
			return StringUtil::parsingDecimalFromString(rest);

		throw(string) "invalid literal operand";
	}

	int getLiteralOperandAddress(string op) {
		try
		{
			return g_curCSECT->litTab.at(op);
		}
		catch (out_of_range& /*err*/) {
			throw(string) "invalid literal operand";
		}
	}

	int operandEvaluation(string operand, currSect* x) {
		int type = findTypeOfExpression(operand);
        setCurrentCSECTValue(x);
		switch (type) {
		case 0: // #
			return getImmediateOperandValue(operand);
		case 1: // @
			return getIndirectOperandValue(operand);
		case 2: // =
			return getLiteralOperandValue(operand);
		case 3:    // Hex Constant
			return StringUtil::parsingHexadecimalFromString(operand.substr(2, operand.length() - 3));
		case 4:    // Byte Constant
			return getByteConstantValue(operand.substr(2, operand.length() - 3));
		case 5:    // int constant
			return StringUtil::parsingDecimalFromString(operand);
		case 6: // expression
			return ExpressionUtil::evaluatingExpression(operand, g_curCSECT, false);
		default:
			throw(string) "invalid operand";
		}
	}
}

namespace ValidateUtil
{
	string g_reg[] = { "A", "X", "L", "B", "S", "T", "F" };
	//A(0), X(1), L(2) are registers present in SIC along with PC(8) and SW(9)
	// But as they can't be modified by user hence not included in  reg[]
	// and SIC/XE contain four additional general purpose registers
	//B(3), S(4), T(5) and F(6).Register F is of 48 bits unlike other registers
	// of 24 bits.

	map<string, char>* g_directives = new map<string, char>();

	int validation(parsedLines pl) {
        insertDirectiveValues();
		string opCode = StringUtil::convertToUpperCase(pl.opCodes);
		map<string, opCodeInfo*>* op = getOperationTable();
		map<string, opCodeInfo*>::iterator it;
		map<string, char>::iterator it2;
		string label = pl.labels;
		cout << "label " << label << endl;
		//    cout << "opcode " << pl.opcode << endl;
		//    cout << "op1 " << pl.operand1 << endl;
		//    cout << "op2 " << pl.operand2 << endl;

		if (checkIfGarbageValue(label)) {

			throw new string("Invalid declaration of Label Field");
		}
		string o1 = StringUtil::convertToUpperCase(pl.operand1);
		string o2 = StringUtil::convertToUpperCase(pl.operand2);
		int ret = -1;

		if (o2.compare("") != 0 && o2.at(0) == ',')
			throw new string("Extra characters");
		label = StringUtil::convertToUpperCase(label);
		it = op->find(label);
		it2 = g_directives->find(label);
		//if label is reserved word
		if (it2 != g_directives->end() || it != op->end()) {
			//        cout << "the wrong label " << l << endl;
			throw new string("Invalid Label field (reserved)");
		}

		it2 = g_directives->find(opCode);
		//IF directive
		if (it2 != g_directives->end()) {
			if (opCode.compare("BYTE") == 0) {
				if (o1.compare("") != 0) {
					if (!checkByteValue(o1) || o2.compare("") != 0)
						throw new string("Invalid operands  for Byte");
				}
				else {
					throw new string("Invalid operands for Byte");
				}
			}
			else if (opCode.compare("START") == 0) {
				if (!StringUtil::ifHexadecimal(o1) || o2.compare("") != 0)
					throw new string("START ERROR");
			}
			else if (opCode.compare("END") == 0) {
				if (o2.compare("") != 0)
					throw new string("END error");
			}
			else if (opCode.compare("EQU") == 0) {
				if (label.compare("") == 0)
					throw new string("Missing label Field in Equate");
				if (o2.compare("") != 0)
					throw new string("Invalid operands , directives");
				if (o1.compare("") == 0)
					throw new string("missing operands , directives");
			}
			else if (opCode.compare("ORG") == 0 || opCode.compare("BASE") == 0) {
				if (label.compare("") != 0)
					throw new string("Label Field should be Empty");
				if (o2.compare("") != 0)
					throw new string("Invalid operands , directives");
				if (o1.compare("") == 0)
					throw new string("missing operands , directives");
			}
			else if (opCode.compare("LTORG") == 0
				|| opCode.compare("NOBASE") == 0) {
				if (label.compare("") != 0)
					throw new string("Label Field should be Empty");
				if (o2.compare("") != 0 || o1.compare("") != 0)
					throw new string("operand fields should be kept empty");
			}
			else if (opCode.compare("CSECT") == 0) {
				if (label.compare("") == 0)
					throw new string("Label Field should not be Empty");
				if (o2.compare("") != 0 || o1.compare("") != 0)
					throw new string("operand fields should be kept empty");
			}
			else if (opCode.compare("EXTDEF") == 0
				|| opCode.compare("EXTREF") == 0) {
				if (label.compare("") != 0)
					throw new string("Label Field should be Empty");
				if (o2.compare("") != 0)
					throw new string("Invalid operand");
				if (!checkIfExt(o1))
					throw new string("Invalid Operand (extdef or extref )");

			}
			else {
				if (o2.compare("") != 0)
					throw new string("Invalid operands , directives");
				if (o1.compare("") == 0)
					throw new string("missing operands , directives");
				if (OperandUtil::findTypeOfExpression(o1) != -1) {
					return -1;
				}
				else if (!StringUtil::ifNumeric(o1))
					throw new string("not numeric operand");
			}

		}
		else if (opCode.at(0) == '+') {
			opCode = opCode.substr(1, opCode.length() - 1);
			it = op->find(opCode);
			if (it == op->end()) {
				throw new string("Invalid opcode field");
			}
			if ((it->second->format) == FORMAT_3_4) {
				if (format34(opCode, o1, o2) == 3)
					ret = 4;
			}
		}
		else {

			it = op->find(opCode);

			if (it == op->end()) {
				throw new string("Invalid opcode 1");
			}
			else {

				if ((it->second->format) == FORMAT_1) {
					ret = formatOne(o1, o2);
				}
				else if (it->second->format == FORMAT_2) {
					ret = formatTwo(opCode, o1, o2);
				}
				else // (it->second->format == FORMAT_3_4)
				{
					ret = format34(opCode, o1, o2);
				}
			}

		}

		return ret;
	}

	int formatOne(string o1, string o2) {
		if (o1.compare("") != 0 || o2.compare("") != 0)
			throw new string("Invalid format 1");
		return 1;
	}
	int formatTwo(string op, string o1, string o2) {
		if (op.compare("CLEAR") == 0 || op.compare("TIXR") == 0) {
			if (o2.compare("") != 0)
				throw new string("Invalid format 2");
			if (!ifReg(o1))
				throw new string("Invalid format 2 / not reg");

		}
		else if (op.compare("SVC") == 0) {
			if (o2.compare("") != 0)
				throw new string("Invalid format 2");
			if (!StringUtil::ifNumeric(o1))
				throw new string("Invalid format 2");
		}
		else if (op.compare("SHIFTL") == 0 || op.compare("SHIFTR") == 0) {
			if (o1.compare("") == 0 || o2.compare("") == 0)
				throw new string("Invalid format 2");
			if (!ifReg(o1))
				throw new string("Invalid format 2");
			if (!StringUtil::ifNumeric(o2))
				throw new string("Invalid format 2");
		}
		else {
			if (!ifReg(o1) || !ifReg(o2))
				throw new string("Invalid format 2");
		}
		return 2;
	}

	int format34(string op, string o1, string o2) {
		if (op.compare("RSUB") == 0) {
			if (o1.compare("") != 0 || o2.compare("") != 0)
				throw new string("Invalid format 3/4");
			else
				return 3;
		}
		else if (o1.at(0) == '=') {
			if (op.compare("LDCH") == 0) {
				if (!checkLiteralValue(o1, false) || o2.compare("") != 0) {
					throw new string("Invalid Literal");
				}
			}
			else {
				if (!checkLiteralValue(o1, true) || o2.compare("") != 0) {
					throw new string("Invalid Literal");
				}
			}
		}
		else { //must check expressions

			if (o2.compare("") != 0)
				if (o2.compare("X") != 0)
					throw new string("Invalid format 3/4");
			if (o1.compare("") == 0)
				throw new string("Invalid format 3/4");

			if (OperandUtil::findTypeOfExpression(o1) == -1)
				throw new string("Invalid Expression");
		}
		return 3;
	}

	bool ifReg(string o) {
		for (int i = 0; i < 7; i++) {
			if (o.compare(g_reg[i]) == 0)
				return true;
		}
		return false;
	}

	void insertDirectiveValues() {

		g_directives->insert(pair<string, char>("BYTE", 'a'));
		g_directives->insert(pair<string, char>("WORD", 'a'));
		g_directives->insert(pair<string, char>("RESB", 'a'));
		g_directives->insert(pair<string, char>("RESW", 'a'));
		g_directives->insert(pair<string, char>("START", 'a'));
		g_directives->insert(pair<string, char>("END", 'a'));
		g_directives->insert(pair<string, char>("ORG", 'a'));
		g_directives->insert(pair<string, char>("EQU", 'a'));
		g_directives->insert(pair<string, char>("BASE", 'a'));
		g_directives->insert(pair<string, char>("NOBASE", 'a'));
		g_directives->insert(pair<string, char>("LTORG", 'a'));
		g_directives->insert(pair<string, char>("CSECT", 'a'));
		g_directives->insert(pair<string, char>("EXTDEF", 'a'));
		g_directives->insert(pair<string, char>("EXTREF", 'a'));
	}

	bool checkByteValue(string x) {
		if (x.at(0) == 'X' || x.at(0) == 'C') {
			if (!(x.length() > 0
				&& (x.at(1) == '\'' && x.at(x.length() - 1) == '\'')))
				throw new string("not a hexadecimal\\character string");
			else {
				return true;
			}
		}
		throw new string("Illegal operand23");
	}

	bool checkLiteralValue(string x, bool isReg) {
		if (x.at(0) == '=') {
			if (x.at(1) == 'X') {
				if (x.length() >= 2) {
					if (x.at(2) == '\'' && x.at(x.length() - 1) == '\'') {
						int numChars = x.length() - 4;
						string inner = x.substr(3, numChars);
						if (!StringUtil::ifHexadecimal(inner))
							throw new string("Invalid operands for Literal");
						if (isReg /*&& numChars == 6*/)
							return true;
						else if (!isReg && numChars == 2)
							return true;
						else
							throw new string("Invalid Literal Length");
					}
				}
				else {
					throw new string("Invalid Literal");
				}
			}
			else if (x.at(1) == 'C') {
				if (x.length() > 0) {
					if (x.at(2) == '\'' && x.at(x.length() - 1) == '\'') {
						int numChars = x.length() - 4;
						if (isReg && numChars == 3)
							return true;
						else if (!isReg && numChars == 1)
							return true;
						else
							throw new string("Invalid Literal Length");
					}
				}
				else {
					throw new string("Invalid Literal");
				}
			}
			else {
				string inner = x.substr(1, x.length());
				if (StringUtil::ifNumeric(inner)) {
					return true;
				}
				else {
					throw new string("Invalid Literal");
				}
			}
		}
		throw new string("Invalid Literal");

		/*if (x.at(0) == 'X' || x.at(0) == 'C')
		{
		if (!(x.length() >0 && (x.at(1) == '\'' && x.at(x.length() - 1) == '\'')))
		throw new string("not a hexadecimal\\character string");
		else {
		int numChars =
		}
		return true;
		}
		throw new string("Illegal operand");
		*/
	}

	bool checkCharacter(char a) {
		int ascii = a;
		if (ascii >= 65 && ascii <= 90)
			return true;
		if (ascii >= 97 && ascii <= 122)
			return true;
		return false;
	}

	bool checkIfGarbageValue(string x) {
		if (!(x.length() == 0)) {
			if (checkIfNumber(x.at(0)))
				return true;
			for (unsigned int i = 0; i < x.length(); i++) {
				if (!checkCharacter(x.at(i)) && !checkIfNumber(x.at(i))) {
					return true;
				}
			}
		}
		return false;
	}

	bool checkIfNumber(char a) {
		int ascii = a;
		if (ascii >= 48 && ascii <= 57)
			return true;
		return false;
	}

	bool checkIfExt(string s) {
		bool flag = false;
		s = StringUtil::trimSpaces(s);
		for (unsigned int i = 0; i < s.length(); i++) {
			if (s.at(i) == ' ')
				return false;
		}
		if (s.at(0) == ',' || s.at(s.length() - 1) == ',')
			return false;
		for (unsigned int i = 0; i < s.length(); i++) {
			if (!checkCharacter(s.at(i)) && !checkIfNumber(s.at(i))) {
				if (s.at(i) != ',')
					return false;
				if (flag)
					return false;

				flag = true;
			}
			else //is character
			{
				if (checkIfNumber(s.at(i)) && flag)
					return false;

				flag = false;
			}
		}
		return true;
	}
}
