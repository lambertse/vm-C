#ifndef VM_VM_IMPL_H__
#define VM_VM_IMPL_H__

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

bool boot_vm();
bool init_apps(char** argv, size_t num_apps);
void trap_store_reg_pc();
uint16_t get_instruction();
bool read_image(const char *image_path);

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

/* ADD
 */
void op_add(uint16_t instr);
void op_and(uint16_t instr);
void op_not(uint16_t instr);
/* BRANCH
 * */
void op_br(uint16_t instr);
/* JUMP 
 * */
void op_jmp(uint16_t instr);
/* JUMP Resister 
 * */
void op_jsr(uint16_t instr); 
/* LOAD 
 * */
void op_ld(uint16_t instr);

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
void op_ldi(uint16_t instr);
/* LOAD Register 
 * */
void op_ldr(uint16_t instr);
/* Load effective addr
 * */
void op_lea(uint16_t instr);

/* Store
 * */
void op_st(uint16_t instr);
/* Store indirect 
 * */
void op_sti(uint16_t instr);
/* Store register
 * */
void op_str(uint16_t instr);

// TRAP
void op_trap_getc(uint16_t instr); 
void op_trap_out(uint16_t instr); 
void op_trap_puts(uint16_t instr); 
void op_trap_in(uint16_t instr); 
void op_trap_putsp(uint16_t instr); 
void op_trap_halt(uint16_t instr);

#endif
