#pragma once

#include <stdint.h>
#include "stdbool.h"

typedef struct CPU
{
    int32_t R0;
    int32_t R1;
    int32_t R2;
    int32_t R3;
    bool Z;
    uint32_t PC;
    uint8_t RAM[256];
} CPU;

#define OPCODE_AMOUNT 12
typedef enum Opcode
{
    NON = 0,
    ERR,
    MOV,
    ADD,
    SUB,
    MUL,
    DIV,
    CMP,
    JMP,
    JIZ,
    JNZ,
    HLT
} Opcode;

typedef enum OperandType
{
    No = 0,
    Register,
    Number,
    Cell,
} OperandType;

typedef struct Operand
{
    int32_t value;
    OperandType type;
} Operand;

typedef struct Command
{
    Opcode opcode;
    Operand Operand1;
    Operand Operand2;
} Command;