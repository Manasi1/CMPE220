#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

#include "Utils.h"

currSect execute_Pass1(vector<parsedLines> vec, bool* hasStarted, bool* hasEnded, bool* isSuccessful);
void execute_Pass2(ofstream* opTabFile, vector<currSect>* cSects);

#endif
