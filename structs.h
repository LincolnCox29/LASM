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

typedef enum Opcode
{
    NON = 0,
    ERR,
    MOV,
    ADD,
    SUB,
    MUL,
    DIV,
    WRM,
    RDM,
    HLT
} Opcode;

typedef enum Register
{
    NO = 0,
    R0,
    R1,
    R2,
    R3
} Register;


typedef struct Command
{
    Opcode opcode;
    Register _register;
    int32_t Operand;
} Command;