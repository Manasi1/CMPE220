#ifndef IO_H_
#define IO_H_

#include "Utils.h"

#define FILE_LINE_LENGTH 63;
#define PASS_2_LENGTH 89;
#define LINE_NUMBER_LENGTH 16;
#define ADDRESS_LENGTH 15;
#define LABEL_LENGTH 9;
#define MNEMONIC_LENGTH 8;
#define OPERANDS_LENGTH 16;
#define FLAGS_LENGTH 15;
#define OPCODE_LENGTH 10;
#define SPACE_CHAR 0x20;
#define ZERO_CHAR 0x30
#define ASTERISK_CHAR 0x2A;
#define DASH_CHAR 0x2D;
#define NEW_LINE_CHAR 0x0A;
#define EQUAL_CHAR 0x3D;
#define TAB_CHAR 0x09;

string lineConstruction(int lineNumber, string label, string op, string operands);

string lineConstructionForPass2(int address, string label, string op_str,
                                string operands, bitset<6> flags, string opCode);

void removeFile(string fileName);

void addLine(ofstream* fileName, string line);

string readLine(ifstream* fileName);

void addHeader(ofstream* fileName);

void addHeaderForPass2(ofstream* fileName);

void addErrorMessage(ofstream* fileName, string errorMessage);

void addBorder(ofstream* fileName);

void addBorderForPass2(ofstream* fileName);

void addSymbolTable(ofstream* file, map<string, symbolInfo*>* theMap);

void addComment(ofstream* file, string comment);

void addHeaderRecord(ofstream* file, int starting_add, int prog_length);

void addTextRecords(ofstream* file, vector<string>* opCodes, vector<locateParsedLine>* lpl);

void addModRecords(ofstream* file, vector<modification>* mods);

void addDefineRecords(ofstream* file, vector<string>* exdefs, map<string, symbolInfo*>* theMap);

void addReferRecords(ofstream* file, vector<string>* exrefs);

void addEndRecords(ofstream* file, string prog_name, int starting_add);

#endif
