#include "OpcodeTable.h"

map<string, opCodeInfo*>* g_opTable = NULL;

static void fillOpTab() {

	opCodeInfo* info;

	info = new opCodeInfo;
    info->format = FORMAT_1;
	info->opCode = 0xC4;
	g_opTable->insert(pair<string, opCodeInfo*>("FIX", info));

	info = new opCodeInfo;
    info->format = FORMAT_1;
	info->opCode = 0xC8;
	g_opTable->insert(pair<string, opCodeInfo*>("NORM", info));

	info = new opCodeInfo;
    info->format = FORMAT_1;
	info->opCode = 0xF8;
	g_opTable->insert(pair<string, opCodeInfo*>("TIO", info));

	info = new opCodeInfo;
    info->format = FORMAT_1;
	info->opCode = 0xF0;
	g_opTable->insert(pair<string, opCodeInfo*>("SIO", info));

	info = new opCodeInfo;
    info->format = FORMAT_1;
	info->opCode = 0xC0;
	g_opTable->insert(pair<string, opCodeInfo*>("FLOAT", info));

	info = new opCodeInfo;
    info->format = FORMAT_1;
	info->opCode = 0xF4;
	g_opTable->insert(pair<string, opCodeInfo*>("HIO", info));

	info = new opCodeInfo;
    info->format = FORMAT_2;
	info->opCode = 0x90;
	g_opTable->insert(pair<string, opCodeInfo*>("ADDR", info));

	info = new opCodeInfo;
    info->format = FORMAT_2;
	info->opCode = 0xB4;
	g_opTable->insert(pair<string, opCodeInfo*>("CLEAR", info));

	info = new opCodeInfo;
    info->format = FORMAT_2;
	info->opCode = 0xA0;
	g_opTable->insert(pair<string, opCodeInfo*>("COMPR", info));

	info = new opCodeInfo;
    info->format = FORMAT_2;
	info->opCode = 0x9C;
	g_opTable->insert(pair<string, opCodeInfo*>("DIVR", info));

	info = new opCodeInfo;
    info->format = FORMAT_2;
	info->opCode = 0x98;
	g_opTable->insert(pair<string, opCodeInfo*>("MULR", info));

	info = new opCodeInfo;
    info->format = FORMAT_2;
	info->opCode = 0xAC;
	g_opTable->insert(pair<string, opCodeInfo*>("RMO", info));

	info = new opCodeInfo;
    info->format = FORMAT_2;
	info->opCode = 0xA4;
	g_opTable->insert(pair<string, opCodeInfo*>("SHIFTL", info));

	info = new opCodeInfo;
    info->format = FORMAT_2;
	info->opCode = 0xA8;
	g_opTable->insert(pair<string, opCodeInfo*>("SHIFTR", info));

	info = new opCodeInfo;
    info->format = FORMAT_2;
	info->opCode = 0x94;
	g_opTable->insert(pair<string, opCodeInfo*>("SUBR", info));

	info = new opCodeInfo;
    info->format = FORMAT_2;
	info->opCode = 0xB0;
	g_opTable->insert(pair<string, opCodeInfo*>("SVC", info));

	info = new opCodeInfo;
    info->format = FORMAT_2;
	info->opCode = 0xB8;
	g_opTable->insert(pair<string, opCodeInfo*>("TIXR", info));

	info = new opCodeInfo;
    info->format = FORMAT_3_4;
	info->opCode = 0x18;
	g_opTable->insert(pair<string, opCodeInfo*>("ADD", info));

	info = new opCodeInfo;
    info->format = FORMAT_3_4;
	info->opCode = 0x58;
	g_opTable->insert(pair<string, opCodeInfo*>("ADDF", info));

	info = new opCodeInfo;
    info->format = FORMAT_3_4;
	info->opCode = 0x40;
	g_opTable->insert(pair<string, opCodeInfo*>("AND", info));

	info = new opCodeInfo;
    info->format = FORMAT_3_4;
	info->opCode = 0x28;
	g_opTable->insert(pair<string, opCodeInfo*>("COMP", info));

	info = new opCodeInfo;
    info->format = FORMAT_3_4;
	info->opCode = 0x88;
	g_opTable->insert(pair<string, opCodeInfo*>("COMPF", info));

	info = new opCodeInfo;
    info->format = FORMAT_3_4;
	info->opCode = 0x24;
	g_opTable->insert(pair<string, opCodeInfo*>("DIV", info));

	info = new opCodeInfo;
    info->format = FORMAT_3_4;
	info->opCode = 0x64;
	g_opTable->insert(pair<string, opCodeInfo*>("DIVF", info));

	info = new opCodeInfo;
    info->format = FORMAT_3_4;
	info->opCode = 0x44;
	g_opTable->insert(pair<string, opCodeInfo*>("OR", info));

	info = new opCodeInfo;
    info->format = FORMAT_3_4;
	info->opCode = 0xD8;
	g_opTable->insert(pair<string, opCodeInfo*>("RD", info));

	info = new opCodeInfo;
    info->format = FORMAT_3_4;
	info->opCode = 0x4C;
	g_opTable->insert(pair<string, opCodeInfo*>("RSUB", info));

	info = new opCodeInfo;
    info->format = FORMAT_3_4;
	info->opCode = 0xEC;
	g_opTable->insert(pair<string, opCodeInfo*>("SSK", info));

	info = new opCodeInfo;
    info->format = FORMAT_3_4;
	info->opCode = 0x0C;
	g_opTable->insert(pair<string, opCodeInfo*>("STA", info));

	info = new opCodeInfo;
    info->format = FORMAT_3_4;
	info->opCode = 0x78;
	g_opTable->insert(pair<string, opCodeInfo*>("STB", info));

	info = new opCodeInfo;
    info->format = FORMAT_3_4;
	info->opCode = 0x54;
	g_opTable->insert(pair<string, opCodeInfo*>("STCH", info));

	info = new opCodeInfo;
    info->format = FORMAT_3_4;
	info->opCode = 0x80;
	g_opTable->insert(pair<string, opCodeInfo*>("STF", info));

	info = new opCodeInfo;
    info->format = FORMAT_3_4;
	info->opCode = 0xD4;
	g_opTable->insert(pair<string, opCodeInfo*>("STI", info));

	info = new opCodeInfo;
    info->format = FORMAT_3_4;
	info->opCode = 0x14;
	g_opTable->insert(pair<string, opCodeInfo*>("STL", info));

	info = new opCodeInfo;
    info->format = FORMAT_3_4;
	info->opCode = 0x7C;
	g_opTable->insert(pair<string, opCodeInfo*>("STS", info));

	info = new opCodeInfo;
    info->format = FORMAT_3_4;
	info->opCode = 0xE8;
	g_opTable->insert(pair<string, opCodeInfo*>("STSW", info));

	info = new opCodeInfo;
    info->format = FORMAT_3_4;
	info->opCode = 0x84;
	g_opTable->insert(pair<string, opCodeInfo*>("STT", info));

	info = new opCodeInfo;
    info->format = FORMAT_3_4;
	info->opCode = 0x10;
	g_opTable->insert(pair<string, opCodeInfo*>("STX", info));

	info = new opCodeInfo;
    info->format = FORMAT_3_4;
	info->opCode = 0x1C;
	g_opTable->insert(pair<string, opCodeInfo*>("SUB", info));

	info = new opCodeInfo;
    info->format = FORMAT_3_4;
	info->opCode = 0x5C;
	g_opTable->insert(pair<string, opCodeInfo*>("SUBF", info));

	info = new opCodeInfo;
    info->format = FORMAT_3_4;
	info->opCode = 0xE0;
	g_opTable->insert(pair<string, opCodeInfo*>("TD", info));

	info = new opCodeInfo;
    info->format = FORMAT_3_4;
	info->opCode = 0x2C;
	g_opTable->insert(pair<string, opCodeInfo*>("TIX", info));

	info = new opCodeInfo;
    info->format = FORMAT_3_4;
	info->opCode = 0xDC;
	g_opTable->insert(pair<string, opCodeInfo*>("WD", info));

	info = new opCodeInfo;
    info->format = FORMAT_3_4;
	info->opCode = 0x3C;
	g_opTable->insert(pair<string, opCodeInfo*>("J", info));

	info = new opCodeInfo;
    info->format = FORMAT_3_4;
	info->opCode = 0x30;
	g_opTable->insert(pair<string, opCodeInfo*>("JEQ", info));

	info = new opCodeInfo;
    info->format = FORMAT_3_4;
	info->opCode = 0x34;
	g_opTable->insert(pair<string, opCodeInfo*>("JGT", info));

	info = new opCodeInfo;
    info->format = FORMAT_3_4;
	info->opCode = 0x38;
	g_opTable->insert(pair<string, opCodeInfo*>("JLT", info));

	info = new opCodeInfo;
    info->format = FORMAT_3_4;
	info->opCode = 0x48;
	g_opTable->insert(pair<string, opCodeInfo*>("JSUB", info));

	info = new opCodeInfo;
    info->format = FORMAT_3_4;
	info->opCode = 0x00;
	g_opTable->insert(pair<string, opCodeInfo*>("LDA", info));

	info = new opCodeInfo;
    info->format = FORMAT_3_4;
	info->opCode = 0x68;
	g_opTable->insert(pair<string, opCodeInfo*>("LDB", info));

	info = new opCodeInfo;
    info->format = FORMAT_3_4;
	info->opCode = 0x50;
	g_opTable->insert(pair<string, opCodeInfo*>("LDCH", info));

	info = new opCodeInfo;
    info->format = FORMAT_3_4;
	info->opCode = 0x70;
	g_opTable->insert(pair<string, opCodeInfo*>("LDF", info));

	info = new opCodeInfo;
    info->format = FORMAT_3_4;
	info->opCode = 0x08;
	g_opTable->insert(pair<string, opCodeInfo*>("LDL", info));

	info = new opCodeInfo;
    info->format = FORMAT_3_4;
	info->opCode = 0x6C;
	g_opTable->insert(pair<string, opCodeInfo*>("LDS", info));

	info = new opCodeInfo;
    info->format = FORMAT_3_4;
	info->opCode = 0x74;
	g_opTable->insert(pair<string, opCodeInfo*>("LDT", info));

	info = new opCodeInfo;
    info->format = FORMAT_3_4;
	info->opCode = 0x04;
	g_opTable->insert(pair<string, opCodeInfo*>("LDX", info));

	info = new opCodeInfo;
    info->format = FORMAT_3_4;
	info->opCode = 0xD0;
	g_opTable->insert(pair<string, opCodeInfo*>("LPS", info));

	info = new opCodeInfo;
    info->format = FORMAT_3_4;
	info->opCode = 0x20;
	g_opTable->insert(pair<string, opCodeInfo*>("MUL", info));

	info = new opCodeInfo;
    info->format = FORMAT_3_4;
	info->opCode = 0x60;
	g_opTable->insert(pair<string, opCodeInfo*>("MULF", info));

	info = NULL;
}

map<string, opCodeInfo*>* getOperationTable() {
	if (g_opTable == NULL) {
		g_opTable = new map<string, opCodeInfo*>();
		fillOpTab();
	}
	return g_opTable;
}

void deleteOperationTable() {
	g_opTable->clear();
	delete g_opTable;
}
