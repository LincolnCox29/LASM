#include "structs.h"
#include "stdio.h"

void printCpuState(CPU* _cpu)
{
    printf("\n-------------------------");
    printf("\n|      CPU STATE       |");
    printf("\n+-----------+----------+");
    printf("\n| Register  | Value    |");
    printf("\n+-----------+----------+");
    printf("\n| R0        | %-8d |", _cpu->R0);
    printf("\n| R1        | %-8d |", _cpu->R1);
    printf("\n| R2        | %-8d |", _cpu->R2);
    printf("\n| R3        | %-8d |", _cpu->R3);
    printf("\n+-----------+----------+");
    printf("\n| ZF        | %-8d |", _cpu->Z);
    printf("\n-------------------------\n");
}

void printMemoryDump(CPU* _cpu) 
{
    printf("\n+-----------------------------------------------------------------+");
    printf("\n|                       Memory Dump (Hex)                         |");
    printf("\n+-----------------------------------------------------------------+");

    for (int row = 0; row < 16; row++) 
    {
        printf("\n| %04X: ", row * 16);
        for (int col = 0; col < 16; col++) 
        {
            if (col % 4 == 0) 
                printf(" ");
            printf("%02X ", _cpu->RAM[row * 16 + col]);
        }
        printf(" | ");
        for (int col = 0; col < 16; col++) 
        {
            uint8_t c = _cpu->RAM[row * 16 + col];
            printf("%c", (c >= 32 && c <= 126) ? c : '.');
        }
        printf(" |");
    }
    printf("\n+-----------------------------------------------------------------+");
}