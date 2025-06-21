#include "stdbool.h"
#include "structs.h"
#include "string.h"
#include "ctype.h"
#include <stdlib.h>
#include "stdio.h"

#include "output.h"

#define DEBUG
#define OPCODE_AMOUNT 10

char* programm = "; calculation test\n"
                "MOV R0 1\n"
                "MOV R1 2\n"
                "MUL R1 2\n"
                "MOV R2 4\n"
                "ADD R2 5\n"
                "SUB R2 1\n"
                "MOV R3 16\n"
                "DIV R3 2\n"
                "; memory test\n"
                "WRM R0 0\n"
                "WRM R1 1\n"
                "WRM R2 2\n"
                "WRM R3 3\n"
                "; hello world\n"
                "MOV R0 72\n"
                "WRM R0 16\n"
                "MOV R0 101\n"
                "WRM R0 17\n"
                "MOV R0 108\n"
                "WRM R0 18\n"
                "WRM R0 19\n"
                "MOV R0 111\n"
                "WRM R0 20\n"
                "MOV R0 32\n"
                "WRM R0 21\n"
                "MOV R0 87\n"
                "WRM R0 22\n"
                "MOV R0 111\n"
                "WRM R0 23\n"
                "MOV R0 114\n"
                "WRM R0 24\n"
                "MOV R0 108\n"
                "WRM R0 25"
                "MOV R0 100\n"
                "WRM R0 26\n"
                 "HLT";

Opcode spotOpcode(char* blob);
Register spotRegister(char* blob);
bool strEquals(char* str1, char* str2, size_t len);
void skipSpace(char** curCharPtrPtr);
int32_t extractDigitOperand(char* blob);
Command parsCommand(char** curCharPtr);
void executionCommand(Command* cmd, CPU* cpu);
int32_t* getRegisterPtr(Register* _register, CPU* _cpu);

int main()
{
    CPU _cpu = (CPU){0};
    char* curCharPtr = programm;
    while (*curCharPtr != '\0')
    {
        Command cmd = parsCommand(&curCharPtr);
        if (cmd.opcode != NON)
            executionCommand(&cmd, &_cpu);
        curCharPtr = strchr(curCharPtr, '\n') + 1;
    }
#ifdef DEBUG
    printf("WARNING: Program does not contain 'HLT' instruction. Execution may run indefinitely or crash.");
#endif // DEBUG

    return 0;
}

Command parsCommand(char** curCharPtr)
{
    Command cmd = { 0 };
    skipSpace(curCharPtr);
    cmd.opcode = spotOpcode(*curCharPtr);
    if (cmd.opcode == NON)
        return (Command) {0};
    (*curCharPtr) += 3;
    skipSpace(curCharPtr);
    cmd._register = spotRegister(*curCharPtr);
    (*curCharPtr) += 2;
    skipSpace(curCharPtr);
    cmd.Operand = extractDigitOperand(*curCharPtr);
    return cmd;
}

void executionCommand(Command* cmd, CPU* _cpu)
{
    int32_t* registerPtr = getRegisterPtr(&cmd->_register, _cpu);
    switch (cmd->opcode)
    {
        case MOV:
            *registerPtr = cmd->Operand;
            break;
        case HLT:
            printCpuState(_cpu);
            printMemoryDump(_cpu);
            exit(0);
            break;
        case ERR:
#ifdef DEBUG
            fprintf(stderr, "ERROR: Unknown command\n");
#endif // DEBUG
            exit(1);
            break;
        case ADD:
            *registerPtr += cmd->Operand;
            break;
        case SUB:
            *registerPtr -= cmd->Operand;
            break;
        case MUL:
            *registerPtr *= cmd->Operand;
            break;
        case DIV:
            *registerPtr /= cmd->Operand;
            break;
        case WRM:
            _cpu->RAM[cmd->Operand] = *registerPtr;
            break;
        case RDM:
            *registerPtr = _cpu->RAM[cmd->Operand];
            break;
    }
}

int32_t* getRegisterPtr(Register* _register, CPU* _cpu)
{
    switch (*_register)
    {
        case R0:
            return &_cpu->R0;
            break;
        case R1:
            return &_cpu->R1;
            break;
        case R2:
            return &_cpu->R2;
            break;
        case R3:
            return &_cpu->R3;
            break;
    }
}

int32_t extractDigitOperand(char* blob)
{
    size_t len = 0;
    for (size_t i = 0; isdigit(blob[i]); i++)
        len++;
    char* digitRow = malloc(len + 1);
    memcpy(digitRow, blob, len);
    digitRow[len] = '\0';
    int32_t num = (int32_t)atoi(digitRow);
    free(digitRow);
    return num;
}

Opcode spotOpcode(char* blob)
{
    if (*blob == '\n' || *blob == ';')
        return NON;
    char opcodes[OPCODE_AMOUNT][3] = { "NON", "ERR", "MOV", "ADD", "SUB", "MUL", "DIV", "WRM", "RDM", "HLT"};
    for (int i = 2; i < OPCODE_AMOUNT; i++)
        if (strEquals(blob, opcodes[i], 3))
            return (Opcode) { i };
    return ERR;
}

Register spotRegister(char* blob)
{
    char registers[5][2] = { "ER", "R0", "R1", "R2", "R3"};
    for (int i = 1; i < 5; i++)
        if (strEquals(blob, registers[i], 2))
            return (Register) { i };
    return (Register) { 0 };
}

bool strEquals(char* str1, char* str2, size_t len)
{
    for (size_t i = 0; i < len; i++)
        if (str1[i] != str2[i])
            return false;
    return true;
}

void skipSpace(char** curCharPtrPtr)
{
    while (**curCharPtrPtr == ' ')
        (*curCharPtrPtr)++;
}