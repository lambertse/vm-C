#ifndef VM_DEFINE_H__
#define VM_DEFINE_H__

// clang-format off

enum {
  REG_R0 = 0,
  REG_R1,
  REG_R2,
  REG_R3,
  REG_R4,
  REG_R5,
  REG_R6,
  REG_R7,
  REG_PC,
  REG_COND,
  REG_COUNT
};

enum {
  OP_BR = 0, /* branch */
  OP_ADD,    /* add  */
  OP_LD,     /* load */
  OP_ST,     /* store */
  OP_JSR,    /* jump register */
  OP_AND,    /* bitwise and */
  OP_LDR,    /* load register */
  OP_STR,    /* store register */
  OP_RTI,    /* unused */
  OP_NOT,    /* bitwise not */
  OP_LDI,    /* load indirect */
  OP_STI,    /* store indirect */
  OP_JMP,    /* jump */
  OP_RES,    /* reserved (unused) */
  OP_LEA,    /* load effective address */
  OP_TRAP    /* execute trap */
};

enum {
  TRAP_GETC = 0x20,  /* get character from keyboard, not echoed onto the terminal */
  TRAP_OUT = 0x21,   /* output a character */
  TRAP_PUTS = 0x22,  /* output a word string */
  TRAP_IN = 0x23,    /* get character from keyboard, echoed onto the terminal */
  TRAP_PUTSP = 0x24, /* output a byte string */
  TRAP_HALT = 0x25   /* halt the program */
};

/*
    The R_COND register stores condition flags which provide information about
   the most recently executed calculation. This allows programs to check logical
   conditions such as if (x > 0) { ... }.
*/
enum { 
  CON_FL_POS = 2 << 0,
  CON_FL_ZERO = 2 << 1,
  CON_FL_NEG = 2 << 2 
};

enum {
  MR_KBSR = 0xFE00, /* keyboard status */
  MR_KBDR = 0xFE02  /* keyboard data */
};

#define MEMORY_MAX (1 << 16)
#define REG_PC_START 0x3000

// clang-format on

#endif
