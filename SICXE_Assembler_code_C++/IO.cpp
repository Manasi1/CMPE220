#include "IO.h"
#include "OpCodeGen.h"

int g_counter = 0;

string lineConstruction(int lineNumber, string label, string op, string operands) {
	int length;
	char spaceChar = SPACE_CHAR;
	char zeroChar = ZERO_CHAR;
	string line;
	string lineNumberStr = StringUtil::convertIntegerToString(++g_counter, false);
	length = LINE_NUMBER_LENGTH;
	string addressStr = StringUtil::convertIntegerToString(lineNumber, true);
	StringUtil::adjustStringLength(&lineNumberStr, length, spaceChar, false);
	length = 6;
	StringUtil::adjustStringLength(&addressStr, length, zeroChar, true);
	length = ADDRESS_LENGTH;
	StringUtil::adjustStringLength(&addressStr, length, spaceChar, false);
	length = LABEL_LENGTH;
	if (op.at(0) == '+')
		length -= 1;
	StringUtil::adjustStringLength(&label, length, spaceChar, false);
	length = MNEMONIC_LENGTH;
	if (op.at(0) == '+')
		length += 1;
	StringUtil::adjustStringLength(&op, length, spaceChar, false);
	length = OPERANDS_LENGTH;
	StringUtil::adjustStringLength(&operands, length, spaceChar, false);
	line = lineNumberStr + addressStr + label + op + operands;
	return line;
}

string lineConstructionForPass2(int address, string label, string op_str,
								string operands, bitset<6> flags, string opCode) {
	int length;
	char spaceChar = SPACE_CHAR;
	char zeroChar = ZERO_CHAR;
	string line;
	string lineNumberStr = StringUtil::convertIntegerToString(++g_counter, false);
	length = LINE_NUMBER_LENGTH;
	string addressStr = StringUtil::convertIntegerToString(address, true);
	StringUtil::adjustStringLength(&lineNumberStr, length, spaceChar, false);
	length = 6;
	StringUtil::adjustStringLength(&addressStr, length, zeroChar, true);
	length = ADDRESS_LENGTH;
	StringUtil::adjustStringLength(&addressStr, length, spaceChar, false);
	length = LABEL_LENGTH;
	if (op_str.at(0) == '+')
		length -= 1;
	StringUtil::adjustStringLength(&label, length, spaceChar, false);
	length = MNEMONIC_LENGTH;
	if (op_str.at(0) == '+')
		length += 1;
	StringUtil::adjustStringLength(&op_str, length, spaceChar, false);
	length = OPERANDS_LENGTH;
	StringUtil::adjustStringLength(&operands, length, spaceChar, false);
	string flags_str = flags.to_string(), mod_flags_str = "";
	if (flags_str.compare("000000") != 0) {
		for (int i = 0; (unsigned)i < flags_str.size(); i++) {
			mod_flags_str += flags_str[i];
			if ((unsigned)i + 1 < flags_str.size())
				mod_flags_str += " ";
		}
	}
	length = FLAGS_LENGTH;
	StringUtil::adjustStringLength(&mod_flags_str, length, spaceChar, false);
	length = OPCODE_LENGTH;
	StringUtil::adjustStringLength(&opCode, length, spaceChar, false);
	line = lineNumberStr + addressStr + label + op_str + operands
		+ mod_flags_str + opCode;
	return line;
}

void removeFile(string fileName) {
	remove(fileName.c_str());

}

void addLine(ofstream* fileName, string line) {
	*fileName << line << endl;
}

string readLine(ifstream* fileName) {
	string line = "";
	getline(*fileName, line);
	return line;
}

void addHeader(ofstream* fileName) {

	int fileLineLength = FILE_LINE_LENGTH;
	int lineNumberLength = LINE_NUMBER_LENGTH;
	int addressLength = ADDRESS_LENGTH;
	int labelLength = LABEL_LENGTH;
	int mnemonicLength = MNEMONIC_LENGTH;
	int operandsLength = OPERANDS_LENGTH;
	char spaceChar = SPACE_CHAR;
	char equalChar = EQUAL_CHAR;
	char hyphen = DASH_CHAR;
	string line = "";
	line = "\t\t\tOutput for Pass 1\n\t\t\tOperation Table\n";
	addLine(fileName, line);
	line = "";
	string temp = "line number";
	StringUtil::adjustStringLength(&temp, lineNumberLength, spaceChar, false);
	line += temp;
	temp = "address";
	StringUtil::adjustStringLength(&temp, addressLength, spaceChar, false);
	line += temp;
	temp = "label";
	StringUtil::adjustStringLength(&temp, labelLength, spaceChar, false);
	line += temp;
	temp = "op";
	StringUtil::adjustStringLength(&temp, mnemonicLength, spaceChar, false);
	line += temp;
	temp = "operands";
	StringUtil::adjustStringLength(&temp, operandsLength, spaceChar, false);
	line += temp;
	addLine(fileName, line);
	line = "";
	StringUtil::adjustStringLength(&line, fileLineLength, hyphen, false);
	addLine(fileName, line);
}

void addHeaderForPass2(ofstream* fileName) {
	int pass2Length = PASS_2_LENGTH;
	int lineNumberLength = LINE_NUMBER_LENGTH;
	int addressLength = ADDRESS_LENGTH;
	int labelLength = LABEL_LENGTH;
	int mnemonicLength = MNEMONIC_LENGTH;
	int operandsLength = OPERANDS_LENGTH;
	int flagsLength = FLAGS_LENGTH;
	int opCodeLength = OPCODE_LENGTH;
	char spaceChar = SPACE_CHAR;
	char equalChar = EQUAL_CHAR;
	char hyphen = DASH_CHAR;
	string line = "";
	line = "\t\t\tOutput for Pass 2\n\t\t\tOperation Table\n";
	addLine(fileName, line);
	line = "";
	string temp = "line number";
	StringUtil::adjustStringLength(&temp, lineNumberLength, spaceChar, false);
	line += temp;
	temp = "address";
	StringUtil::adjustStringLength(&temp, addressLength, spaceChar, false);
	line += temp;
	temp = "label";
	StringUtil::adjustStringLength(&temp, labelLength, spaceChar, false);
	line += temp;
	temp = "op";
	StringUtil::adjustStringLength(&temp, mnemonicLength, spaceChar, false);
	line += temp;
	temp = "operands";
	StringUtil::adjustStringLength(&temp, operandsLength, spaceChar, false);
	line += temp;
	temp = "n i x b p e";
	StringUtil::adjustStringLength(&temp, flagsLength, spaceChar, false);
	line += temp;
	temp = "opcode";
	StringUtil::adjustStringLength(&temp, opCodeLength, spaceChar, false);
	line += temp;
	addLine(fileName, line);
	line = "";
	StringUtil::adjustStringLength(&line, pass2Length, hyphen, false);
	addLine(fileName, line);
}

void addErrorMessage(ofstream* fileName, string errorMessage) {
	vector<string> vecOfStrs = StringUtil::splitDelimiter(errorMessage, '\n');

	for (unsigned int i = 0; i < vecOfStrs.size(); i++) {
		errorMessage = vecOfStrs.at(i);
		int errLength = errorMessage.length();
		int offset = 19;
		int excess;
		char spaceChar = SPACE_CHAR;
		char asterisk = ASTERISK_CHAR;

		StringUtil::adjustStringLength(&errorMessage, errLength + 3, asterisk, true);
		StringUtil::adjustStringLength(&errorMessage, errLength + offset, spaceChar, true);
		excess = 63 - errLength;
		StringUtil::adjustStringLength(&errorMessage, errLength + excess, spaceChar, false);
		addLine(fileName, errorMessage);
	}
}

void addBorder(ofstream* fileName) {
	char newLine = NEW_LINE_CHAR;
	int fileLineLength = FILE_LINE_LENGTH;
	string line = "";
	char asterickChar = DASH_CHAR;
	for (int i = 0; i < fileLineLength; i++)
		line += asterickChar;
	addLine(fileName, newLine + line + newLine);
}

void addBorderForPass2(ofstream* fileName) {
	char newLine = NEW_LINE_CHAR;
	int fileLineLength = PASS_2_LENGTH;
	string line = "";
	char asterickChar = DASH_CHAR;
	for (int i = 0; i < fileLineLength; i++)
		line += asterickChar;
	addLine(fileName, newLine + line + newLine);
}

void addSymbolTable(ofstream* file, map<string, symbolInfo*>* theMap) {
	if (theMap->empty())
		return;
	char spaceChar = SPACE_CHAR;
	char dashChar = DASH_CHAR;
	char equalChar = EQUAL_CHAR;
	char zeroChar = ZERO_CHAR;
	char hyphen = DASH_CHAR;

	string temp, headerLine = "";
	addLine(file, "\tSymbol Table\n");
	temp = "";
	StringUtil::adjustStringLength(&temp, 33, hyphen, false);
	addLine(file, temp);
	temp = "\t|name";
	StringUtil::adjustStringLength(&temp, 11, spaceChar, false);
	headerLine += temp;
	//    addLine(file, temp);
	temp = "address";
	StringUtil::adjustStringLength(&temp, 10, spaceChar, false);
	headerLine += temp;
	temp = "Abs/Rel";
	temp += "|";
	headerLine += temp;
	addLine(file, headerLine);
	temp = "\t|";
	StringUtil::adjustStringLength(&temp, 28, dashChar, false);
	temp += "|";
	addLine(file, temp);
	for (map<string, symbolInfo*>::const_iterator it = theMap->begin(); it
                                                                        != theMap->end(); it++) {
		string symbol = "\t|" + it->first;
		symbolInfo* n = it->second;
		string address = StringUtil::convertIntegerToString(n->address, true);
		string rel = "";
		if (n->isRelocatable)
			rel = "Rel";
		else
			rel = "Abs";
		StringUtil::adjustStringLength(&symbol, 11, spaceChar, false);
		string line = symbol;
		StringUtil::adjustStringLength(&address, 4, zeroChar, true);
		StringUtil::adjustStringLength(&address, 5, spaceChar, true);
		StringUtil::adjustStringLength(&address, 10, spaceChar, false);
		line += address;
		StringUtil::adjustStringLength(&address, 10, spaceChar, true);
		StringUtil::adjustStringLength(&rel, 7, spaceChar, true);
		rel += "|";
		line += rel;
		addLine(file, line);
	}
	temp = "";
	StringUtil::adjustStringLength(&temp, 33, hyphen, false);
	addLine(file, temp);
}

void addComment(ofstream* file, string comment) {
	int lineNumberLength = LINE_NUMBER_LENGTH;
	int addressLength = ADDRESS_LENGTH;
	char spaceChar = SPACE_CHAR;
	StringUtil::adjustStringLength(&comment,
		comment.size() + lineNumberLength + addressLength, spaceChar, true);
	addLine(file, comment);
}

void addHeaderRecord(ofstream* file, int starting_add, int prog_length) { // 1
	char spaceChar = SPACE_CHAR;
	char zeroChar = ZERO_CHAR;

	string line = "";
	addLine(file, line);
	line = "H^";
	StringUtil::adjustStringLength(&line, 11, spaceChar, true);
	string starting_add_str = StringUtil::convertIntegerToString(starting_add, true);
	StringUtil::adjustStringLength(&starting_add_str, 6, zeroChar, true);
	line += starting_add_str;
	line += "^";
	string prog_length_str = StringUtil::convertIntegerToString(prog_length, true);
	StringUtil::adjustStringLength(&prog_length_str, 6, zeroChar, true);
	line += prog_length_str;
	addLine(file, line);
	line = "";
	StringUtil::adjustStringLength(&line, 11, spaceChar, true);
	line += "Loading Address: ";
	line += starting_add_str;
	line += "\n";
	addLine(file, line);
}

void addTextRecords(ofstream* file, vector<string>* opCodes, vector<locateParsedLine>* lpl) { // 4
	char zeroChar = ZERO_CHAR;
	char spaceChar = SPACE_CHAR;
	int start_address = 0;
	int end_address = 0;
	string start_address_str;
	string length_str;

	string line = "";
	string temp_line = "";
	addLine(file, line);

	for (unsigned int i = 0; i< opCodes->size();) {
		if ((*opCodes)[i].compare("") == 0 || (*lpl)[i].p.labels.compare("*") == 0) {
			i++;
			continue;
		}

		line = "T^";
		StringUtil::adjustStringLength(&line, 11, spaceChar, true);
		start_address = (*lpl)[i].loc;
		start_address_str = StringUtil::convertIntegerToString(start_address, true);
		StringUtil::adjustStringLength(&start_address_str, 6, zeroChar, true);
		line += start_address_str;
		int x = 0;
		string temp = "";
		while (1) {
			if (i >= (*opCodes).size() || x >= 5)
				break;

			if ((*opCodes)[i].compare("") == 0 || (*lpl)[i].p.labels.compare("*") == 0) {
				i++;
				break;
			}
			temp += "^";
			temp += (*opCodes)[i];
			if (i + 1 < (*opCodes).size()) end_address = (*lpl)[i + 1].loc;
			else end_address = (*lpl)[i].loc;
			i++;
			x++;
		}
		length_str = StringUtil::convertIntegerToString(end_address - start_address, true);
		StringUtil::adjustStringLength(&length_str, 2, zeroChar, true);
		line += "^";
		line += length_str;
		line += temp;
		addLine(file, line);
	}
}

void addModRecords(ofstream* file, vector<modification>* mods) { // 5
	char zeroChar = ZERO_CHAR;
	char spaceChar = SPACE_CHAR;
	int address = 0, hBytes = 0;
	string address_str, hBytes_str;
	string line = "";
	addLine(file, line);
	for (int i = 0; (unsigned)i < mods->size(); i++) {
		modification mod = (*mods)[i];
		if (mod.isExtRef) {
			// multiple mod records
			for (int j = 0; (unsigned)j < mod.refs.size(); j++) {
				line = "M^";
				StringUtil::adjustStringLength(&line, 11, spaceChar, true);
				address = mod.address;
				address_str = StringUtil::convertIntegerToString(address, true);
				line += address_str;
				line += "^";
				hBytes = mod.hBytes;
				hBytes_str = StringUtil::convertIntegerToString(hBytes, false);
				StringUtil::adjustStringLength(&hBytes_str, 2, zeroChar, true);
				line += hBytes_str;
				line += "^";
				line += mod.signs[j];
				line += mod.refs[j];
				addLine(file, line);
			}
		}
		else {
			line = "M^";
			StringUtil::adjustStringLength(&line, 11, spaceChar, true);
			address = mod.address;
			address_str = StringUtil::convertIntegerToString(address, true);
			line += address_str;
			line += "^";
			hBytes = mod.hBytes;
			hBytes_str = StringUtil::convertIntegerToString(hBytes, false);
			StringUtil::adjustStringLength(&hBytes_str, 2, zeroChar, true);
			line += hBytes_str;
			addLine(file, line);
		}
	}
	addLine(file, "");
}

void addDefineRecords(ofstream* file, vector<string>* exdefs, map<string, symbolInfo*>* theMap) { // 2
	char zeroChar = ZERO_CHAR;
	char spaceChar = SPACE_CHAR;

	int address = 0;
	int i = 0;
	string address_str;
	string line = "";
	addLine(file, line);
	for (; (unsigned)i < exdefs->size(); i++) {
		line = "D^";
		StringUtil::adjustStringLength(&line, 11, spaceChar, true);
		do {
			if ((unsigned)i >= exdefs->size()) break;
			string s = (*exdefs)[i];
			symbolInfo* sInfo = theMap->at(s);
			address = sInfo->address;
			address_str = StringUtil::convertIntegerToString(address, true);
			StringUtil::adjustStringLength(&address_str, 6, zeroChar, true);
			line += (*exdefs)[i];
			line += "^";
			line += address_str;
			if ((i + 1) % 5 != 0 && (unsigned)(i + 1) < exdefs->size()) line += "^";
			i++;
		} while (i % 5 != 0);
		if ((unsigned)i >= exdefs->size())line += "\n";
		i--;
		addLine(file, line);
	}
}

void addReferRecords(ofstream* file, vector<string>* exrefs) { // 3
	char spaceChar = SPACE_CHAR;
	int i = 0;

	string line = "";
	addLine(file, line);
	for (; (unsigned)i < exrefs->size(); i++) {
		line = "R^";
		StringUtil::adjustStringLength(&line, 11, spaceChar, true);
		do {
			if ((unsigned)i >= exrefs->size()) break;
			line += (*exrefs)[i];
			if ((i + 1) % 5 != 0 && (unsigned)(i + 1) < exrefs->size()) line += "^";
			i++;
		} while (i % 5 != 0);
		if ((unsigned)i >= exrefs->size()) line += "\n";
		i--;
		addLine(file, line);
	}
}

void addEndRecords(ofstream* file, string prog_name, int starting_add) {
	/// todo Implement End Record Here
	char spaceChar = SPACE_CHAR;
	char zeroChar = ZERO_CHAR;

	string line = "";
	addLine(file, line);
	line = "E^";
	StringUtil::adjustStringLength(&line, 11, spaceChar, true);
	string starting_add_str = StringUtil::convertIntegerToString(starting_add, true);
	StringUtil::adjustStringLength(&starting_add_str, 6, zeroChar, true);
	line += starting_add_str;
	addLine(file, line);
	line = "";
	StringUtil::adjustStringLength(&line, 11, spaceChar, true);

}
