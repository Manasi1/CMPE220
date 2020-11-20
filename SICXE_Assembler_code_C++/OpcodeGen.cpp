#include "OpcodeGen.h"
#include "OpcodeTable.h"
#include "IO.h"
map<string, unsigned char>* g_registers = NULL;

map<string, unsigned char>* getRegMap() {
	if (g_registers != NULL)
		return g_registers;

	g_registers = new map<string, unsigned char>();
	g_registers->insert(pair<string, unsigned char>("A", 0));
	g_registers->insert(pair<string, unsigned char>("X", 1));
	g_registers->insert(pair<string, unsigned char>("L", 2));
	g_registers->insert(pair<string, unsigned char>("B", 3));
	g_registers->insert(pair<string, unsigned char>("S", 4));
	g_registers->insert(pair<string, unsigned char>("T", 5));
	g_registers->insert(pair<string, unsigned char>("F", 6));
	return g_registers;
}

opCode opCodeGeneration(parsedLines parLines, int locationCounter, currSect* current) {
	opCode toRet;
	string modOp = (parLines.opCodes).at(0) == '+' ? (parLines.opCodes).substr(1) : parLines.opCodes;
	modOp = StringUtil::convertToUpperCase(modOp);
	string op1 = StringUtil::convertToUpperCase(parLines.operand1);
	string op2 = StringUtil::convertToUpperCase(parLines.operand2);

	opCodeInfo* info;
	try {
		info = getOperationTable()->at(modOp); //Only if instruction, not directive
	}
	catch (out_of_range& orr) { //Directive
		int value = 0;
		if (modOp.compare("WORD") == 0) {
			toRet.format = 11;

			value = OperandUtil::operandEvaluation(parLines.operand1, current);

			if (value >= -(1 << 23) && value <= (1 << 23) - 1) {
				toRet.displacement = value;
			}
			else {
				throw new string("Too large to store");
			}
		}
		else if (modOp.compare("BYTE") == 0) {
			toRet.format = 22;

			if (op1.at(0) == 'C') { //max 7 ascii characters +3 'C' '\'' '\''
				if (op1.size() <= 10) {
					toRet.displacement = OperandUtil::operandEvaluation(parLines.operand1, current);
				}
				else {
					throw new string("Too large to store");
				}
			}
			else if (op1.at(0) == 'X') {
				if (op1.size() <= 17) { //max 14 hexa characters +3 'X' '\'' '\''
					toRet.displacement = OperandUtil::operandEvaluation(parLines.operand1, current);
				}
				else {
					throw new string("Too large to store");
				}
			}
		}
		else if (modOp.at(0) == '=') {
			toRet.format = 22;
			toRet.displacement = OperandUtil::getLiteralOperandValue(parLines.opCodes);
		}
		else {
			toRet.format = 33;
		}
		return toRet;
	}

	unsigned char format = info->format;

	if (format == FORMAT_1) {
		toRet.operation = info->opCode;
		toRet.displacement = 0;
		toRet.format = FORMAT_1;
		toRet.flags[5] = 1;
		toRet.flags[4] = 1;
		toRet.flags[3] = 0;
		toRet.flags[2] = 0;
		toRet.flags[1] = 0;
		toRet.flags[0] = 0;
	}
	else if (format == FORMAT_2) {
		toRet.operation = info->opCode;
		toRet.reg1 = getRegMap()->at(op1);
		toRet.reg2 = (op2).compare("") != 0 ? g_registers->at(op2) : 0;
		toRet.format = FORMAT_2;
		toRet.flags[5] = 1;
		toRet.flags[4] = 1;
		toRet.flags[3] = 0;
		toRet.flags[2] = 0;
		toRet.flags[1] = 0;
		toRet.flags[0] = 0;
	}
	else {
		// format 3 or 4
		toRet.operation = info->opCode;
		toRet.format = FORMAT_3_4;

		if (modOp.compare("RSUB") == 0) {
			toRet.displacement = 0;
			toRet.flags[5] = 1; //n
			toRet.flags[4] = 1; //i
			toRet.flags[3] = 0; //x
			return toRet;
		}
		toRet.flags[0] = (parLines.opCodes).at(0) == '+' ? 1 : 0; //e

		if (op1.at(0) == '#') { //Immediate
			toRet.flags[5] = 0; //n
			toRet.flags[4] = 1; //i
			toRet.flags[3] = 0; //x
		}
		else if ((parLines.operand1).at(0) == '@') { //Indirect
			toRet.flags[5] = 1; //n
			toRet.flags[4] = 0; //i
			toRet.flags[3] = 0; //x
		}
		else {
			toRet.flags[5] = 1; //n
			toRet.flags[4] = 1; //i
			toRet.flags[3] = (op2).compare("X") == 0 ? 1 : 0; //x
		}

		int address = 0;
		try {
			if (OperandUtil::findTypeOfExpression(parLines.operand1) == 2)
				address = OperandUtil::getLiteralOperandAddress(parLines.operand1);
			else
				address = OperandUtil::operandEvaluation(parLines.operand1, current);
		}
		catch (string& err) {
			throw err;
		}

		if (toRet.flags[0]) { //Extended? (20 bits)
			toRet.flags[2] = 0; //b
			toRet.flags[1] = 0; //p
			if (address >= -(1 << 19) && address <= (1 << 19) - 1)
				toRet.displacement = address;
			else
				throw new string("Too large to store");
		}
		else { //only 12 bits
			toRet.flags[2] = 0; //b
			toRet.flags[1] = 1; //p
			int disp = 0;
			if (OperandUtil::findTypeOfExpression(parLines.operand1) == 6
				|| OperandUtil::findTypeOfExpression(parLines.operand1) == 1
				|| OperandUtil::findTypeOfExpression(parLines.operand1) == 2
				|| (OperandUtil::findTypeOfExpression(parLines.operand1) == 0
					&& OperandUtil::ifValidOperand(parLines.operand1)))
				disp = address - locationCounter;
			else
				disp = address; //Numeric value after #
			if (disp >= -(1 << 11) && disp <= (1 << 11) - 1) { //Because PC is signed
				toRet.displacement = disp;
			}
			else {
				throw new string("Too large to store");
			}
		}
	}
	return toRet;
}

string opCodeCalculation(opCode opCd) {
	string returnedString = "";
	if (opCd.format == 11) { // reserved word, length = 6
		string final_opCode = StringUtil::convertIntegerToString(opCd.displacement, true);
		StringUtil::adjustStringLength(&final_opCode, 6, '0', true);
		return final_opCode;
	}
	else if (opCd.format == 22) { // reserved byte, length = 2
		string final_opCode = StringUtil::convertIntegerToString(opCd.displacement, true);
		StringUtil::adjustStringLength(&final_opCode, 2, '0', true);
		return final_opCode;
	}
	else if (opCd.format == 33) {
		return returnedString;
	}
	else if (opCd.format == FORMAT_1) {
		returnedString = StringUtil::convertIntegerToString(opCd.operation, true);
	}
	else if (opCd.format == FORMAT_2) {
		returnedString = ((StringUtil::convertIntegerToString(opCd.operation, true)
			+ StringUtil::convertIntegerToString(opCd.reg1, false)) + StringUtil::convertIntegerToString(opCd.reg2, false));
	}
	else { // format = 3 or 4
		int ni_val = 0, xbpe_val = 0;
        convertBinaryToDecimal(opCd.flags, &ni_val, &xbpe_val);
		string disp_str = StringUtil::convertIntegerToString(opCd.displacement, true);
		if (opCd.flags[0] == 0) {
			StringUtil::adjustStringLength(&disp_str, 3, '0', true);
			disp_str = disp_str.substr(disp_str.length() - 3);
		}
		else {
			StringUtil::adjustStringLength(&disp_str, 5, '0', true);
			disp_str = disp_str.substr(disp_str.length() - 5);
		}
		returnedString = ((StringUtil::convertIntegerToString(opCd.operation + ni_val, true)
			+ StringUtil::convertIntegerToString(xbpe_val, true)) + disp_str);
	}
	return returnedString;
}

void convertBinaryToDecimal(bitset<6> flags, int* ni_val, int* xbpe_val) { // takes all 6 flags
	bitset<4> xbpe_flags;
	for (int i = 0; i < 4; i++)
		xbpe_flags[i] = flags[i];

	bitset<2> ni_flags;
	for (int i = 0; i < 2; i++)
		ni_flags[i] = flags[i + 4];

	*ni_val = *xbpe_val = 0;
	stringstream ss_1, ss_2;
	ss_1 << uppercase << std::hex << ni_flags.to_ulong();
	ss_1 >> std::hex >> *ni_val;
	ss_2 << std::hex << uppercase << xbpe_flags.to_ulong();
	ss_2 >> std::hex >> *xbpe_val;
}
