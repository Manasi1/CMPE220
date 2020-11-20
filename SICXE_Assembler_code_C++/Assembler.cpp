#include "Assembler.h"
#include "OpcodeTable.h"
#include "IO.h"
#include "OpcodeGen.h"

string errorToDisplay;
vector<parsedLines> givenLiterals;
bool doesGivenLiteralExists;

int handlingBytes(string byte) {
	string str = "";
	if (byte.at(0) == 'x' || byte.at(0) == 'X' || byte.at(0) == 'c' || byte.at(0) == 'C') {
		if (byte.at(1) == '\'' && byte.at(byte.length() - 1) == '\'') {

			for (unsigned int i = 2; i < byte.length() - 1; i++) {
                str += byte.at(i);
			}
		}
		else {
            errorToDisplay += "\nnot a hexadecimal/character string";
			return 0;
		}
	}
	else {
        errorToDisplay += "\nillegal operand!!!";
		return 0;
	}

	if (byte.at(0) == 'c' || byte.at(0) == 'C')
		return str.length();

	return str.length() % 2 == 0 ? str.length() / 2 : str.length() / 2 + 1;
}

void addingLabelsToSymbolTable(string strng, currSect* current) {
	if (strng.compare("") != 0) {
		try {
			current->symTab.at(strng);
            errorToDisplay += "\nDuplicate Label Definition";
		}
		catch (out_of_range& oor) {
			symbolInfo* c = new symbolInfo;
			c->address = current->locCtr;
			c->isRelocatable = true;
			current->symTab.insert(pair<string, symbolInfo*>(strng, c));
		}
	}
}

void writeAsFormatted(parsedLines parLine, currSect* currentSec) {
	toWrite x;
	x.data = lineConstruction((*currentSec).locCtr, parLine.labels, parLine.opCodes,
							  StringUtil::mergingStrings(parLine.operand1, parLine.operand2, ','));
	x.type = 1;
	currentSec->dataSect.push_back(x);
	locateParsedLine temp;
	temp.p = parLine;
	temp.loc = currentSec->locCtr;
	currentSec->locpl.push_back(temp);
}

void incrementLocationCounter(currSect* current, int increment) {
    current->locCtr += increment;
	if (current->locCtr > 0x33039) {
        errorToDisplay += "\nout of range address";
	}
}

void incrementAndWrite(int increment, parsedLines parLine, currSect* current, bool* hasStarted,
                       bool* hasEnded) {
	if (!*hasStarted) {
		*hasStarted = true;
        current->locCtr = 0;
        writeAsFormatted(parLine, current);
        addingLabelsToSymbolTable(parLine.labels, current);
        incrementLocationCounter(current, increment);
	}
	else if (*hasStarted && !*hasEnded) {
        writeAsFormatted(parLine, current);
        addingLabelsToSymbolTable(parLine.labels, current);
        incrementLocationCounter(current, increment);
	}
	else if (*hasEnded) {
        errorToDisplay += "\nDefinition After END";
	}
}

bool ifExternalReferenceExists(parsedLines parLine, currSect* current, vector<string>* references,
                               vector<char>* signs) {
	if (parLine.operand1.find("(") != string::npos
        && parLine.operand1.find(")") != string::npos) {
        errorToDisplay += "\nBrackets are not allowed";
		return false;
	}

	vector<string> all = StringUtil::generalizedSplitter(parLine.operand1, "+-*/");
	bool answer = false;
	string myOperand = parLine.operand1;
	for (unsigned int i = 0; i < all.size(); i++) {
		if (find(current->extRef.begin(), current->extRef.end(), all[i])
			!= current->extRef.end()) {
			references->push_back(all[i]);
			answer = true;
			int position = myOperand.find(all[i]);
			myOperand.substr(position + all[i].size());
			if (position == 0) {
				signs->push_back('+');
			}
			else {
				char temp = myOperand.at(position - 1);
				if (temp == '+' || temp == '-')
					signs->push_back(temp);
			}
		}
	}
	return answer;
}

void handleModificationRecords(parsedLines parLine, currSect* current) {
	modification mod;
	bool hasP = StringUtil::trimSpaces(parLine.opCodes).at(0) == '+';
	bool hasExRef = ifExternalReferenceExists(parLine, current, &(mod.refs),
                                              &(mod.signs));

	if (hasP) {
		mod.address = current->locCtr + 1;
		mod.hBytes = 5;
		if (hasExRef) {
			mod.isExtRef = true;
		}
		else {
			mod.isExtRef = false;
		}
		current->mods.push_back(mod);
	}
	else {
		if (hasExRef) {
			mod.isExtRef = true;
			if (StringUtil::convertToUpperCase(parLine.opCodes).compare("BYTE") == 0) {
				mod.address = current->locCtr;
				mod.hBytes = 2 * handlingBytes(parLine.operand1);
				current->mods.push_back(mod);
			}
			else if (StringUtil::convertToUpperCase(parLine.opCodes).compare("WORD") == 0) {
				mod.address = current->locCtr;
				mod.hBytes = 6;
				current->mods.push_back(mod);
			}
			else {
                errorToDisplay += "\nUsing ExtRef with no +";
			}
		}
	}
}
bool checkForLabels(vector<string> labels, currSect* current) {
	if (labels.size() == 1 && labels[0] == "*")
		return true;
	for (unsigned int i = 0; i < labels.size(); i++) {
		if (labels[i].at(0) == '#' || labels[i].at(0) == '@')
			labels[i] = labels[i].substr(1);
		if (!StringUtil::ifNumeric(labels[i])) {
			try {
				current->symTab.at(labels[i]);
			}
			catch (out_of_range& x) {
				return false;
			}
		}
	}
	return true;
}
void handlingExpressions(parsedLines parLine, currSect * current) {
	if (OperandUtil::findTypeOfExpression(parLine.operand1) != -1) {

		vector<string> labels = StringUtil::generalizedSplitter(parLine.operand1, "+-*/");
		if (parLine.operand1.compare("*") == 0)
			labels.push_back("*");
		if (!checkForLabels(labels, current)) {
            errorToDisplay += "\nUNIDENTIFIED LABELS";
			return;
		}
		else {
			int value;
			try {
				value = OperandUtil::operandEvaluation(parLine.operand1, current);
			}
			catch (string* e) {
                errorToDisplay += "\nUnable to evaluate the given expression";
				delete e;
				return;
			}
			symbolInfo* c = new symbolInfo;
			c->address = value;
			c->isRelocatable = true;
			current->symTab.insert(pair<string, symbolInfo*>(parLine.labels, c));
            writeAsFormatted(parLine, current);
		}
	}
	else {

        errorToDisplay += "\nInvalid Expression";
	}
}

void handlingOrgs(parsedLines parLine, currSect* current) {
	if (ExpressionUtil::validatingExpression(parLine.operand1)) {
		vector<string> labels = StringUtil::generalizedSplitter(parLine.operand1, "+-*/");
		if (!checkForLabels(labels, current)) {
            errorToDisplay += "\n UNIDENTIFIED LABELS";
			return;
		}
		else {
			int value;
			try {
				value = ExpressionUtil::evaluatingExpression(parLine.operand1, current);
			}
			catch (string* e) {
                errorToDisplay += "\nUnable to evaluate the given expression";
				delete e;
				return;
			}
            writeAsFormatted(parLine, current);
			current->locCtr = value;
		}
	}
	else {
        errorToDisplay += "\nInvalid Expression";
	}
}

bool ifLit(string pl) {
	if (pl.at(0) == '=')
		return true;
	return false;
}

int lengthOfLiteral(string literals, currSect* current) {
	int x;

	int y = OperandUtil::operandEvaluation(literals, current);
	if (literals.at(1) == 'C') {
		string s = literals.substr(3, literals.length() - 4);
		x = s.length();
	}
	else if (literals.at(1) == 'X') {
		string s = literals.substr(3, literals.length() - 4);
		x = s.length() * 2;
	}
	else
		x = 3;
	return x;
}

void writingLiterals(currSect* current) {
    doesGivenLiteralExists = false;
	for (unsigned int i = 0; i < givenLiterals.size(); i++) {
		parsedLines p;
		p.labels = "*";
		p.opCodes = StringUtil::trimSpaces(givenLiterals[i].operand1);
		p.operand1 = "";
		p.operand2 = "";
		p.error = "";
        writeAsFormatted(p, current);
		current->litTab.insert(
			pair<string, int>(StringUtil::trimSpaces(givenLiterals[i].operand1), current->locCtr));
		//  Increment by the literal length
        incrementLocationCounter(current, lengthOfLiteral(givenLiterals[i].operand1, current));

	}
	givenLiterals.clear();
}

void addingExternalRef(parsedLines parLine, currSect* current) {
	if (current->extRef.size() == 0) {
		current->extRef = StringUtil::generalizedSplitter(parLine.operand1, ",");
		for (unsigned int i = 0; i < current->extRef.size(); i++) {
			current->extRef[i] = StringUtil::trimSpaces(current->extRef[i]);
		}
	}
	else
        errorToDisplay +=
		"\nEXTREF can be written only once in a control section";
}

void addingExternalDef(parsedLines parLine, currSect* current) {
	if (current->extDef.size() == 0) {
		current->extDef = StringUtil::generalizedSplitter(parLine.operand1, ",");
		for (unsigned int i = 0; i < current->extDef.size(); i++) {
			current->extDef[i] = StringUtil::trimSpaces(current->extDef[i]);
		}
	}
	else
        errorToDisplay +=
		"\nEXTDEF can be written only once in a control section";
}

void getLocationCounterIncr(parsedLines pl, currSect* current, bool* started, bool* ended) {
	int incr = ValidateUtil::validation(pl);
	if (pl.operand1.length() > 0 && ifLit(pl.operand1)) {
		bool add = true;
		for (unsigned int y = 0; y < givenLiterals.size(); y++)
		{
			if (givenLiterals[y].operand1.compare(pl.operand1) == 0) {
				add = false;
				break;
			}
		}
		if (add)
		{
			givenLiterals.push_back(pl);
		}
        doesGivenLiteralExists = true;
	}
    handleModificationRecords(pl, current);
	if (incr == -1) { // Directive
		string upper = StringUtil::convertToUpperCase(pl.opCodes);
		if (upper.compare("START") == 0) {
			if (!*started) {
				*started = true;
                incrementLocationCounter(current, StringUtil::parsingHexadecimalFromString(pl.operand1));
                writeAsFormatted(pl, current);
                addingLabelsToSymbolTable(pl.labels, current);
			}
			else {
                errorToDisplay += "\nMisplaced START";
			}
		}
		else if (upper.compare("END") == 0) {
			if (!*started) {
                errorToDisplay += "\nEnded before START";
			}
			else if (*started && !*ended) {
				*ended = true;
                incrementLocationCounter(current, 0);
                writeAsFormatted(pl, current);
                addingLabelsToSymbolTable(pl.labels, current);
			}
			else if (*ended) {
                errorToDisplay += "\nMultiple END clauses.";
			}
		}
		else if (upper.compare("BYTE") == 0) {
            incrementAndWrite(handlingBytes(pl.operand1), pl, current, started, ended);
		}
		else if (upper.compare("WORD") == 0) {
            incrementAndWrite(3, pl, current, started, ended);
		}
		else if (upper.compare("RESB") == 0) {
            incrementAndWrite(StringUtil::parsingDecimalFromString(pl.operand1), pl, current, started,
                              ended);
		}
		else if (upper.compare("RESW") == 0) {
            incrementAndWrite(StringUtil::parsingDecimalFromString(pl.operand1) * 3, pl, current, started,
                              ended);
		}
		else if (upper.compare("EQU") == 0) {
            handlingExpressions(pl, current);
		}
		else if (upper.compare("ORG") == 0) {
            handlingOrgs(pl, current);
		}
		else if (upper.compare("LTORG") == 0) {
            writeAsFormatted(pl, current);
            writingLiterals(current);
		}
		else if (upper.compare("EXTREF") == 0) {
            writeAsFormatted(pl, current);
            addingExternalRef(pl, current);
		}
		else if (upper.compare("EXTDEF") == 0) {
            writeAsFormatted(pl, current);
            addingExternalDef(pl, current);
		}
	}
	else { //Operations (formats)
		if (!*started) {
			*started = true;
			current->locCtr = 0;
            writeAsFormatted(pl, current);
            addingLabelsToSymbolTable(pl.labels, current);
            incrementLocationCounter(current, incr);
		}
		else if (*started && !*ended) {
            writeAsFormatted(pl, current);
            addingLabelsToSymbolTable(pl.labels, current);
            incrementLocationCounter(current, incr);
		}
		else if (*ended) {
            errorToDisplay += "\nDefinition After END";
		}
	}
}

bool ifComment(parsedLines pl) {
	return StringUtil::trimSpaces(pl.opCodes).at(0) == '.';
}

bool ifEmptyLine(parsedLines pl) {
	if (pl.labels.compare("") == 0 && pl.opCodes.compare("") == 0
        && pl.operand1.compare("") == 0 && pl.operand2.compare("") == 0)
		return true;

	return false;
}

currSect execute_Pass1(vector<parsedLines> v, bool* started, bool* ended,
                       bool* successfull) {
	parsedLines pl;
	currSect current;
	current.locCtr = 0;
	givenLiterals.clear();
    doesGivenLiteralExists = false;
	for (unsigned int i = 0; i < v.size(); i++) {
		pl = v[i];
		if (ifEmptyLine(pl)) {
			continue;
		}
		if (ifComment(pl)) {
			toWrite x;
			x.data = pl.opCodes;
			x.type = 0;
			current.dataSect.push_back(x);
			continue;
		}
		try {
            getLocationCounterIncr(pl, &current, started, ended);
		}
		catch (string* e) {
            writeAsFormatted(pl, &current);
			*(successfull) = false;
            errorToDisplay += "\n";
            errorToDisplay += *e;
			delete e;
		}
		if (errorToDisplay.compare("") != 0) {
			toWrite x;
			*successfull = false;
			x.data = errorToDisplay;
			x.type = -1;
			current.dataSect.push_back(x);
            errorToDisplay = "";
		}
	}
	if (doesGivenLiteralExists)
        writingLiterals(&current);
	return current;
}

void execute_Pass2(ofstream* opFile, vector<currSect>* cSects) {
    errorToDisplay = "";
	addBorderForPass2(opFile);
	addHeaderForPass2(opFile);
	vector<vector<string> > objectCodes;
	bool success = true;
	int starting = cSects->at(0).locpl.at(0).loc;
	currSect cc = cSects->at(cSects->size() - 1);
	int ending = cc.locpl.at(cc.locpl.size() - 1).loc;
	for (unsigned int i = 0; i < cSects->size(); i++) {
		vector<string> ii;
		objectCodes.push_back(ii);
		vector<locateParsedLine> v = cSects->at(i).locpl;
		for (unsigned j = 0; j < v.size() - 1; j++) {
			opCode x;
			string objectCode = "";
			try {
				x = opCodeGeneration(v[j].p, v[j + 1].loc, &(cSects->at(i)));
				objectCode = opCodeCalculation(x);
			}
			catch (string* err) {
                errorToDisplay += "\n";
                errorToDisplay += *err;
				success = false;
				delete err;
			}
			catch (string& err) {
                errorToDisplay += "\n";
                errorToDisplay += err;
				success = false;
			}

			string operands = StringUtil::mergingStrings(v[j].p.operand1, v[j].p.operand2,
                                                         ',');
			string line = lineConstructionForPass2(v[j].loc, v[j].p.labels,
												   v[j].p.opCodes, operands, x.flags, objectCode);

			objectCodes[i].push_back(objectCode);
			addLine(opFile, line);

			if (errorToDisplay.compare("") != 0) {
				addErrorMessage(opFile, errorToDisplay);
                errorToDisplay = "";
			}
		}
	}

	if (!success) return;

	opFile->close();

	string out2 = "objectProgram.txt";
	removeFile(out2.c_str());
	opFile->open(out2.c_str(), std::ios::app);

	for (unsigned int i = 0; i < cSects->size(); i++) {
		addHeaderRecord(opFile, starting, ending - starting);
		addDefineRecords(opFile, &(cSects->at(i).extDef),
						 &(cSects->at(i).symTab));
		addReferRecords(opFile, &(cSects->at(i).extRef));
		addTextRecords(opFile, &(objectCodes[i]), &(cSects->at(i).locpl));
		addModRecords(opFile, &(cSects->at(i).mods));
		addEndRecords(opFile, "", starting);
		addBorderForPass2(opFile);
	}

	return;
}
