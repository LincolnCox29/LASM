# LASM

## Overview
This project is a simple CPU emulator that can parse and execute a custom assembly language (LASM). The emulator supports basic arithmetic operations, data movement, comparisons, and conditional jumps.

## Features
- **Registers**: 4 general-purpose registers (R0-R3)
- **Flags**: Zero Flag (ZF) and Carry Flag (CF)
- **Memory**: 256-byte RAM
- **Supported Operations**:
  - Data movement (MOV)
  - Arithmetic (ADD, SUB, MUL, DIV)
  - Comparison (CMP)
  - Conditional jumps (JMP, JIZ, JNZ, JIA, JIL, JAE, JLE)
  - Program control (HLT)

## Input File Format
Input files must have `.lasm` extension and contain one instruction per line.

### Instruction Syntax
```
OPCODE OPERAND1, OPERAND2
```
Where:
- `OPCODE` is a 3-letter operation code
- `OPERAND1` and `OPERAND2` are operands (format depends on operation)

### Operand Types
- **Register**: `R0`, `R1`, `R2`, `R3`
- **Memory cell**: `#123` (decimal address)
- **Immediate value**: `123` (decimal number)
- **Pointer**: `@` prefix (e.g., `@R0` dereferences R0)

## Example Programs
https://github.com/LincolnCox29/LASM/blob/main/test/hello%20world.lasm
https://github.com/LincolnCox29/LASM/blob/main/test/fibonacci.lasm
https://github.com/LincolnCox29/LASM/blob/main/test/factorial.lasm

## Debugging
When compiled with `DEBUG` defined, the emulator will:
- Warn if program lacks HLT instruction
- Print error messages for unknown commands

## Output
The emulator displays:
1. CPU state (register values and flags)
2. Memory dump (hex and ASCII representation)

## Error Handling
The emulator exits with different codes for various errors:
- 1: Unknown command
- 2: No input file specified
- 3: Too many arguments
- 4: Invalid file extension
- 5: File opening error
- 6: Invalid memory operation

## Limitations
- Fixed memory size (256 bytes)
- Limited register set (4 registers)
- Basic error handling

## Future Improvements
- Implement stack operations
- Add debugging features
- Add work with graphics
- Add UI version