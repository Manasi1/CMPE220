#include "Utils.h"
#include "OpcodeTable.h"
#include "IO.h"
#include "Assembler.h"

using namespace std;

ifstream inputFile;
ofstream outputFile;

map<string, opCodeInfo*>* mainOpTab;

void openInitialStreams(string in, string out) {
	// This function opens the file for reading and writing data
	inputFile.open(in.c_str());
	removeFile(out.c_str());
	outputFile.open(out.c_str(), std::ios::app);
}

bool isCSect(parsedLines pl) {
	//This function checks if Control section is encountered or not.
	return (StringUtil::convertToUpperCase(pl.opCodes).compare("CSECT") == 0) ? true : false;
}

parsedLines readAndParse() {
	string before = readLine(&inputFile);
	string x = StringUtil::trimSpaces(before);
	return StringUtil::parsingLines(x);
}

void finish() {
	// This function is called in last to close all opened file streams.
	outputFile.close();
	inputFile.close();
    deleteOperationTable();
}

int main(int argc, char **argv) {

	if (argc != 2) {
		//Checks whether the correct number of arguments are supplied or not.
		// If incorrect number of arguments are supplied than program
		//terminates after printing error message.
		cout << "\aInvalid number of parameters" << endl;
		return 1;
	}

	printf("Assembler Started\n");
	openInitialStreams(argv[1], "objPgmCode.txt");
	printf("Input Files is being read form %s\n", argv[1]);
	bool successfullPass1 = true;
	addHeader(&outputFile);
	mainOpTab = getOperationTable();
	vector<parsedLines> currCSect;
	vector<currSect> cSects;
	parsedLines pl;

	bool started = false;
	bool ended = false;

	while (!inputFile.eof()) {
		pl = readAndParse();

		if (!isCSect(pl)) {
			currCSect.push_back(pl);
			continue;
		}

		if (isCSect(pl)) {
			currSect old = execute_Pass1(currCSect, &started, &ended,
                                         &successfullPass1);
			cSects.push_back(old);
			currCSect.clear(); //Already Handled statements
			currCSect.push_back(pl); //Add 'currSect' to the new parsedLines
		}

	}

	cSects.push_back(execute_Pass1(currCSect, &started, &ended, &successfullPass1));
	currCSect.clear();

	//Write that text to a file
	for (unsigned int i = 0; i < cSects.size(); i++) {
		for (unsigned int j = 0; j < cSects[i].dataSect.size(); j++) {
			toWrite x = cSects[i].dataSect.at(j);
			if (x.type == -1) {
				addErrorMessage(&outputFile, x.data);
			}
			else if (x.type == 0) {
				addComment(&outputFile, x.data);
			}
			else if (x.type == 1) {
				addLine(&outputFile, x.data);
			}
			else {
			}
		}
		addBorder(&outputFile);
		addSymbolTable(&outputFile, &cSects[i].symTab);
	}

	if (!ended) {
		//Check whether program is having a END Clause or not.
		addErrorMessage(&outputFile, "The program has no END clause");
	}

	if (!successfullPass1) {
		finish();
		return 0;
	}

    execute_Pass2(&outputFile, &cSects);
	printf("Check output files!!)\n");
	finish();
	return 0;
}
