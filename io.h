#pragma once

#include "structs.h"

void printMemoryDump(CPU* _cpu);
void printCpuState(CPU* _cpu);
char* parsArgs(int argc, char* argv[]);