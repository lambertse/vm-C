#ifndef VM_OPCODE_IMPLEMENATION_H__
#define VM_OPCODE_IMPLEMENATION_H__

#include "vm/define.h"
#include "vm/input_buffering.h"
#include <stdint.h>
#include <stdio.h>

static uint16_t vm_mem[MEMORY_MAX];
static uint16_t vm_reg[REG_COUNT];

//
#define EXTRACT_BITS(instr, start_bit, num_bits)                               \
  (((instr) >> (start_bit)) & ((1 << (num_bits)) - 1))
#define GET_OPCODE(instr) EXTRACT_BITS(instr, 12, 4)
#define GET_DR(instr) EXTRACT_BITS(instr, 9, 3)
#define GET_SR1(instr) EXTRACT_BITS(instr, 6, 3)
#define GET_SR2(instr) EXTRACT_BITS(instr, 0, 3)
#define GET_IMM5(instr) EXTRACT_BITS(instr, 0, 5)
#define GET_OFFSET6(instr) EXTRACT_BITS(instr, 0, 6)
#define GET_PCOFFSET9(instr) EXTRACT_BITS(instr, 0, 9)
#define GET_PCOFFSET11(instr) EXTRACT_BITS(instr, 0, 11)
//

// Extend the 5 bits into 16 bits with sign
static uint16_t __sign_extend(uint16_t x, int bit_count);
// Any time a value is written to reg, we need to update the flag to indicate
// its sign
static void __update_reg_cond(uint16_t x);
static uint16_t __mem_read(uint16_t addr);
static void __mem_write(uint16_t addr, uint16_t val);

/* ADD
 * There are 2 modes:
 *  * Register mode:   ADD R2 R0 R1
 *  * Immediate mode:  ADD R0 R0 1
 */
void op_add(uint16_t instr) {
  // Get the immediate flag
  uint16_t im_flag = EXTRACT_BITS(instr, 5, 1);
  uint16_t r0 = GET_DR(instr);
  uint16_t r1 = GET_SR1(instr);

  if (im_flag) {
    uint16_t imm5 = __sign_extend(GET_IMM5(instr), 5);
    vm_reg[r0] = vm_reg[r1] + imm5;
  } else {
    uint16_t r2 = GET_SR2(instr);
    vm_reg[r0] = vm_reg[r1] + vm_reg[r2];
  }
  __update_reg_cond(r0);
}

void op_and(uint16_t instr) {
  // Get the immediate flag
  uint16_t im_flag = EXTRACT_BITS(instr, 5, 1);
  uint16_t r0 = GET_DR(instr);
  uint16_t r1 = GET_SR1(instr);

  if (im_flag) {
    uint16_t imm5 = __sign_extend(GET_IMM5(instr), 5);
    vm_reg[r0] = vm_reg[r1] & imm5;
  } else {
    uint16_t r2 = GET_SR2(instr);
    vm_reg[r0] = vm_reg[r1] & vm_reg[r2];
  }
  __update_reg_cond(r0);
}

void op_not(uint16_t instr) {
  uint16_t r0 = GET_DR(instr);
  uint16_t r1 = GET_SR1(instr);

  vm_reg[r0] = ~vm_reg[r1];
  __update_reg_cond(r0);
}

/* BRANCH
 * */
void op_br(uint16_t instr) {
  uint16_t pc_offset = __sign_extend(EXTRACT_BITS(instr, 0, 9), 9);
  uint16_t cond_flag = EXTRACT_BITS(instr, 9, 3);
  if (cond_flag & vm_reg[REG_COND]) {
    vm_reg[REG_PC] += pc_offset;
  }
}

/* JUMP 
 * */
void op_jmp(uint16_t instr) {
  uint16_t r1 = GET_SR1(instr);
  vm_reg[REG_PC] = vm_reg[r1];
}

/* JUMP Resister 
 * */
void op_jsr(uint16_t instr) {
  uint16_t long_flag = EXTRACT_BITS(instr, 11, 1);
  vm_reg[REG_R7] = vm_reg[REG_PC];

  if (long_flag) {
    // jsr
    uint16_t pc_offset = __sign_extend(EXTRACT_BITS(instr, 0, 11), 11);
    vm_reg[REG_PC] += pc_offset;
  } else {
    // jsrr
    uint16_t r1 = GET_SR1(instr);
    vm_reg[REG_PC] = vm_reg[r1];
  }
}

/* LOAD 
 * */
void op_ld(uint16_t instr) {
  // quite simple compare to load indirect
  uint16_t r0 = GET_DR(instr);
  uint16_t pc_offset = __sign_extend(EXTRACT_BITS(instr, 0, 9), 9);
  vm_reg[r0] = __mem_read(vm_reg[REG_PC] + pc_offset);
  __update_reg_cond(r0);
}

/* LDI: Load indirect - This instruction is used to load a value from a location
 * in memory into a register
 *
 *
 *
// In memory it may be layed out like this:
//
// Address Label      Value
// 0x123:  far_data = 0x456
// ...
// 0x456:  string   = 'a'

// if PC was at 0x100
// LDI R0 0x023
// would load 'a' into R0

 * */
void op_ldi(uint16_t instr) {
  uint16_t r0 = GET_DR(instr);
  uint16_t pc_offset = __sign_extend(EXTRACT_BITS(instr, 0, 9), 9);
  vm_reg[r0] = __mem_read(__mem_read(vm_reg[REG_PC] + pc_offset));
  __update_reg_cond(r0);
}

/* LOAD Register 
 * */
void op_ldr(uint16_t instr) {
  uint16_t r0 = GET_DR(instr);
  uint16_t r1 = GET_SR1(instr);
  uint16_t pc_offset = __sign_extend(EXTRACT_BITS(instr, 0, 6), 6);
  vm_reg[r0] = __mem_read(vm_reg[r1] + pc_offset);
  __update_reg_cond(r0);
}

/* Load effective addr
 * */
void op_lea(uint16_t instr) {
  uint16_t r0 = GET_DR(instr);
  uint16_t pc_offset = __sign_extend(EXTRACT_BITS(instr, 0, 9), 9);
  vm_reg[r0] = vm_reg[REG_PC] + pc_offset;
  __update_reg_cond(r0);
}

/* Store
 * */
void op_st(uint16_t instr) {
  uint16_t r0 = GET_DR(instr);
  uint16_t pc_offset = __sign_extend(EXTRACT_BITS(instr, 0, 9), 9);
  __mem_write(vm_reg[REG_PC] + pc_offset, vm_reg[r0]);
}

/* Store indirect 
 * */
void op_sti(uint16_t instr) {
  uint16_t r0 = GET_DR(instr);
  uint16_t pc_offset = __sign_extend(EXTRACT_BITS(instr, 0, 9), 9);
  __mem_write(__mem_read(vm_reg[REG_PC] + pc_offset), vm_reg[r0]);
}

/* Store register
 * */
void op_str(uint16_t instr) {
  uint16_t r0 = GET_DR(instr);
  uint16_t r1 = GET_SR1(instr);
  uint16_t offset = __sign_extend(EXTRACT_BITS(instr, 0, 6), 6);
  __mem_write(vm_reg[r1] + offset, vm_reg[r0]);
}

// TRAP

void op_trap_getc(uint16_t instr) {
  vm_reg[REG_R0] = (uint16_t)getchar();
  __update_reg_cond(REG_R0);
}

void op_trap_out(uint16_t instr) {
  putc((char)vm_reg[REG_R0], stdout);
  fflush(stdout);
}

void op_trap_puts(uint16_t instr) {
  uint16_t *c = vm_mem + vm_reg[REG_R0];
  while (*c) {
    putc((char)*c, stdout);
    ++c;
  }
  fflush(stdout);
}

void op_trap_in(uint16_t instr) {
  printf("Enter a character: ");
  char c = getchar();
  putc(c, stdout);
  fflush(stdout);
  vm_reg[REG_R0] = (uint16_t)c;
  __update_reg_cond(REG_R0);
}

void op_trap_putsp(uint16_t instr) {
  /* one char per byte (two bytes per word)
                                here we need to swap back to
                                big endian format */
  uint16_t *c = vm_mem + vm_reg[REG_R0];
  while (*c) {
    char char1 = (*c) & 0xFF;
    putc(char1, stdout);
    char char2 = (*c) >> 8;
    if (char2)
      putc(char2, stdout);
    ++c;
  }
  fflush(stdout);
}

void op_trap_halt(uint16_t instr) {
  puts("HALT");
  fflush(stdout);
  exit(0);
}

//

// =================
//  functions
uint16_t __sign_extend(uint16_t x, int bit_count) {
  if ((x >> (bit_count - 1)) & 1) {
    x |= (0xFFFF << bit_count);
  }
  return x;
}

void __update_reg_cond(uint16_t r) {
  if (vm_reg[r] == 0) {
    vm_reg[REG_COND] = CON_FL_ZERO;
  } else if (vm_reg[r] >> 15) {
    /* a 1 in the left-most bit indicates negative */
    vm_reg[REG_COND] = CON_FL_NEG;
  } else {
    vm_reg[REG_COND] = CON_FL_POS;
  }
}

uint16_t __mem_read(uint16_t addr) {
  if (addr == MR_KBSR) {
    if (check_key()) {
      vm_mem[MR_KBSR] = (1 << 15);
      vm_mem[MR_KBDR] = getchar();
    } else {
      vm_mem[MR_KBSR] = 0;
    }
  }
  return vm_mem[addr];
}

void __mem_write(uint16_t addr, uint16_t val) { vm_mem[addr] = val; }

#endif
