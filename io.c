#include "structs.h"
#include "stdio.h"
#include "string.h"
#include <stdlib.h>

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

static char* FileToString(FILE* file)
{
    if (!file)
        return NULL;
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);
    char* buffer = (char*)malloc(size + 1);
    if (!buffer)
    {
        fclose(file);
        return NULL;
    }
    size_t bytes_read = fread(buffer, 1, size, file);
    buffer[bytes_read] = '\0';
    fclose(file);
    return buffer;
}

char* parsArgs(int argc, char* argv[])
{
    if (argc != 2)
    {
        if (argc < 2)
        {
            fprintf(stderr, "ERROR: No input file specified\n");
            fprintf(stderr, "USAGE: LASM <pathToProgram>\n");
            exit(2);
            {
                fprintf(stderr, "ERROR: Too many arguments\n");
                fprintf(stderr, "USAGE: LASM <pathToProgram>\n");
                exit(3);
            }
        }
    }
    const char* filename = argv[1];
    const char* dot = strrchr(argv[1], '.');
    if (!dot || strcmp(dot, ".lasm") != 0)
    {
        fprintf(stderr, "ERROR: Input file must have .lasm extension\n");
        exit(4);
    }
    FILE* file = fopen(filename, "r");
    if (!file)
    {
        fprintf(stderr, "Error: Could not open file '%s'\n", filename);
        perror("Details");
        exit(5);
    }
    return FileToString(file);
}