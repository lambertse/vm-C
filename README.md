# LC-3 Virtual Machine

A virtual machine implementation based on the LC-3 (Little Computer 3) architecture, written in C.

## Overview

LC-3 is an educational computer architecture for teaching assembly language programming. This VM executes LC-3 machine code by simulating the instruction set and memory model.

## Features

- Complete LC-3 instruction set (16 opcodes)
- 8 general-purpose registers + PC + condition flags
- 65,536 memory locations (16-bit each)
- Memory-mapped keyboard I/O
- TRAP routines for system calls

## Supported Instructions

**Arithmetic & Logic**
- `ADD`, `AND`, `NOT`

**Control Flow**
- `BR` (conditional branch), `JMP`, `JSR/JSRR`

**Memory Operations**
- `LD`, `LDI`, `LDR`, `LEA`, `ST`, `STI`, `STR`

**TRAP Routines**
- `GETC`, `OUT`, `PUTS`, `IN`, `PUTSP`, `HALT`

## Building and Running

```bash
# Compile
mkdir build
cd build
cmake ..
cmake --build .

# Run a program
./vm-lc3 program.obj
```

## Architecture

**Registers**
- R0-R7: General purpose registers
- PC: Program counter
- COND: Condition flags (N/Z/P)

**Memory**
- 65,536 x 16-bit locations
- Memory-mapped I/O at KBSR/KBDR

## Example Program

```assembly
.ORIG x3000
    LEA R0, HELLO
    PUTS
    HALT
HELLO .STRINGZ "Hello, World!"
.END
```

## References

- [LC-3 VM Tutorial](https://www.jmeiners.com/lc3-vm/)

