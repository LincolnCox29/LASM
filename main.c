#include "stdbool.h"
#include "structs.h"
#include "string.h"
#include "ctype.h"
#include <stdlib.h>
#include "stdio.h"
#include "io.h"

#define DEBUG

Opcode spotOpcode(char* blob);
bool strEquals(char* str1, char* str2, size_t len);
void skipSpace(char** curCharPtrPtr);
Operand extractOperand(char** blob, CPU* _cpu);
Command parsCommand(char** curCharPtr, CPU* _cpu);
void executionCommand(Command* cmd, CPU* cpu);
inline void operandTypeDef(Operand* operand, char** blob);
void Jump(Command* cmd, CPU* _cpu, char** blob, char* programm);

int main(int argc, char* argv[])
{
    char* programm = parsArgs(argc, argv);
    CPU _cpu = (CPU){0};
    char* curCharPtr = programm;
    while (*curCharPtr != '\0')
    {
        Command cmd = parsCommand(&curCharPtr, &_cpu);
        if (cmd.opcode != NON)
        {
            if (cmd.opcode > 7 && cmd.opcode < 15)
            {
                Jump(&cmd, &_cpu, &curCharPtr, programm);
                continue;
            }
            else
                executionCommand(&cmd, &_cpu);
        }
        curCharPtr = strchr(curCharPtr, '\n') + 1;
    }
#ifdef DEBUG
    printf("WARNING: Program does not contain 'HLT' instruction. Execution may run indefinitely or crash.");
#endif // DEBUG

    return 0;
}

Command parsCommand(char** curCharPtr, CPU* _cpu)
{
    Command cmd = { 0 };
    skipSpace(curCharPtr);
    cmd.opcode = spotOpcode(*curCharPtr);
    if (cmd.opcode == NON || cmd.opcode == HLT)
        return cmd;
    (*curCharPtr) += 3;
    skipSpace(curCharPtr);
    cmd.Operand1 = extractOperand(curCharPtr, _cpu);
    if (cmd.opcode > 7 && cmd.opcode < 15)
        return cmd;
    skipSpace(curCharPtr);
    cmd.Operand2 = extractOperand(curCharPtr, _cpu);
    return cmd;
}

void Jump(Command* cmd, CPU* _cpu, char** blob, char* programm)
{
    if (
        (cmd->opcode == JLE && (_cpu->Z || _cpu->C)) ||
        (cmd->opcode == JAE && (_cpu->Z || !_cpu->C)) ||
        (cmd->opcode == JIA && !_cpu->Z && !_cpu->C ) ||
        (cmd->opcode == JNZ && !_cpu->Z) ||
        (cmd->opcode == JIL && _cpu->C) ||
        (cmd->opcode == JIZ && _cpu->Z) ||
        (cmd->opcode == JMP))
    {
        char* newPos = programm;
        int targetLine = cmd->Operand1.value;
        for (int i = 0; i < targetLine; i++)
        {
            newPos = strchr(newPos, '\n');
            if (!newPos)
                break;
            newPos++;
        }
        *blob = newPos;
    }
    else
        *blob = strchr(*blob, '\n') + 1;
}

void executionCommand(Command* cmd, CPU* _cpu)
{
    int32_t *registerPtr = &(_cpu->R0) + cmd->Operand1.value;

    switch (cmd->opcode)
    {
        case MOV:
            if (cmd->Operand1.type == Register)
                *registerPtr = getOperandValue(_cpu, &cmd->Operand2);
            else if (cmd->Operand1.type == Cell && cmd->Operand2.type != Cell)
                _cpu->RAM[cmd->Operand1.value] = getOperandValue(_cpu, &cmd->Operand2);
            else
                exit(6);
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
            *registerPtr += getOperandValue(_cpu, &cmd->Operand2);
            break;
        case SUB:
            *registerPtr -= getOperandValue(_cpu, &cmd->Operand2);
            break;
        case MUL:
            *registerPtr *= getOperandValue(_cpu, &cmd->Operand2);
            break;
        case DIV:
            *registerPtr /= getOperandValue(_cpu, &cmd->Operand2);
            break;
        case CMP:
            _cpu->Z = (getOperandValue(_cpu, &cmd->Operand1) == getOperandValue(_cpu, &cmd->Operand2));
            _cpu->C = (getOperandValue(_cpu, &cmd->Operand1) < getOperandValue(_cpu, &cmd->Operand2));
            break;
    }
}

Operand extractOperand(char** blob, CPU* _cpu)
{
    bool isPtr = false;
    if (**blob == '@')
    {
        isPtr = true;
        (*blob)++;
    }
    Operand operand;
    operandTypeDef(&operand, blob);
    size_t len = 0;
    for (size_t i = 0; isdigit((*blob)[i]); i++)
        len++;
    char* digitRow = malloc(len + 1);
    memcpy(digitRow, *blob, len);
    digitRow[len] = '\0';
    operand.value = (int32_t)atoi(digitRow);
    free(digitRow);
    (*blob) += len;
    if (operand.type == Number)
    {
        (*blob)--;
        return operand;
    }
    if (isPtr)
    {
        switch (operand.type)
        {
            case Register:
                operand.value = *(&_cpu->R0 + operand.value);
                break;
            case Cell:
                operand.value = _cpu->RAM[operand.value];
                break;
        }
        operand.type = Cell;
    }
    return operand;
}

inline void operandTypeDef(Operand* operand, char** blob)
{
    switch (**blob)
    {
        case '#':
            operand->type = Cell;
            (*blob)++;
            break;
        case 'R':
            operand->type = Register;
            (*blob)++;
            break;
        default:
            operand->type = Number;
            break;
    }
}

Opcode spotOpcode(char* blob)
{
    if (*blob == '\n' || *blob == ';')
        return NON;
    char opcodes[OPCODE_AMOUNT][3] = { 
        "NON", "ERR", "MOV", "ADD", "SUB", "MUL", "DIV", 
        "CMP", 
        "JMP", "JIZ", "JNZ", "JIA", "JIL", "JAE", "JLE", 
        "HLT"
    };
    for (int i = 2; i < OPCODE_AMOUNT; i++)
        if (strEquals(blob, opcodes[i], 3))
            return (Opcode) { i };
    return ERR;
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

int32_t getOperandValue(CPU* cpu, Operand* op) 
{
    switch (op->type) 
    {
        case Register: return *(&cpu->R0 + op->value);
        case Number:   return op->value;                
        case Cell:     return cpu->RAM[op->value];      
        default:       return 0;                        
    }
}