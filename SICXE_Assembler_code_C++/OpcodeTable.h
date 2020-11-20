#ifndef OPCODE_TABLE_H_
#define OPCODE_TABLE_H_

#include "DataStructures.h"

const unsigned char FORMAT_1 = 1;
const unsigned char FORMAT_2 = 2;
const unsigned char FORMAT_3_4 = 3;

void deleteOperationTable();
map<string, opCodeInfo*>* getOperationTable();

#endif
