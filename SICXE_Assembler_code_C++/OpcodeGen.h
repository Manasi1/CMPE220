#ifndef OPCODE_GEN_H_
#define OPCODE_GEN_H_

#include "Utils.h"

opCode opCodeGeneration(parsedLines parLines, int locationCounter, currSect* current);

string opCodeCalculation(opCode opCd);

void convertBinaryToDecimal(bitset<6> flags, int* ni_val, int* xbpe_val);

#endif
