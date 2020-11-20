#ifndef DATA_STRUCTURES_H_
#define DATA_STRUCTURES_H_

#include <bitset>
#include <map>
#include <string>
#include <vector>

using namespace std;

struct opCodeInfo {
    unsigned char format;
	unsigned char opCode;
};

struct parsedLines {
	string labels;
	string opCodes;
	string operand1;
	string operand2;
	string error;
};

struct opCode {
	unsigned char operation;
	int displacement;

	bitset<6> flags; //  n i x b p e -> 1 1 0 0 0 0
	unsigned char format; // 1, 2, 3 or 4

	unsigned char reg1;
	unsigned char reg2;
};

struct symbolInfo {
	int address;
	bool isRelocatable;
};

struct toWrite {
	string data;
	int type;
	/*
	* -1 for errors
	*  0 for comments
	*  1 for formatted lines
	*  */
};

struct modification {
	int address;
	int hBytes;

	bool isExtRef;
	vector<char> signs;
	vector<string> refs;
};
struct locateParsedLine {
	parsedLines p;
	int loc;
};
//+ --> modification record.
struct currSect {
	string name;
	int locCtr;
	vector<string> extRef;
	vector<string> extDef;
	map<string, int> litTab;
	vector<toWrite> dataSect;
	vector<locateParsedLine> locpl;
	vector<modification> mods;
	map<string, symbolInfo*> symTab;
};

#endif
