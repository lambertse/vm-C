#include "vm/vm.h"
#include "vm/define.h"

int main(int argc, char *argv[]) {
  // if (argc < 2) {
  //   printf("vm-lc3 <program-path>\n");
  //   exit(2);
  // }
  // for (int i = 1; i < argc; i++) {
  //   __read_image(argv[i]);
  // }
  __read_image("/Users/tri.le/src/opensource/lambertse/vm-C/app/2048.obj");
  signal(SIGINT, handle_interrupt);
  disable_input_buffering();

  vm_reg[REG_COND] = CON_FL_ZERO;
  vm_reg[REG_PC] = REG_PC_START;

  int running = 1;
  while (running) {
    uint16_t instr = __mem_read(vm_reg[REG_PC]++);
    uint16_t op = EXTRACT_BITS(instr, 12, 4);
    switch (op) {
    case OP_ADD: {
      op_add(instr);
      break;
    }
    case OP_AND: {
      op_and(instr);
      break;
    }
    case OP_NOT: {
      op_not(instr);
      break;
    }
    case OP_BR: {
      op_br(instr);
      break;
    }
    case OP_JMP: {
      op_jmp(instr);
      break;
    }
    case OP_JSR: {
      op_jsr(instr);
      break;
    }
    case OP_LD: {
      op_ld(instr);
      break;
    }
    case OP_LDI: {
      op_ldi(instr);
      break;
    }
    case OP_LDR: {
      op_ldr(instr);
      break;
    }
    case OP_LEA: {
      op_lea(instr);
      break;
    }
    case OP_ST: {
      op_st(instr);
      break;
    }
    case OP_STI: {
      op_sti(instr);
      break;
    }
    case OP_STR: {
      op_str(instr);
      break;
    }
    case OP_TRAP: {
      vm_reg[REG_R7] = vm_reg[REG_PC];
      switch (instr & 0xFF) {
      case TRAP_GETC: {
        op_trap_getc(instr);
        break;
      }
      case TRAP_HALT: {
        op_trap_halt(instr);
        break;
      }
      case TRAP_IN: {
        op_trap_in(instr);
        break;
      }
      case TRAP_OUT: {
        op_trap_out(instr);
        break;
      }
      case TRAP_PUTS: {
        op_trap_puts(instr);
        break;
      }
      case TRAP_PUTSP: {
        op_trap_putsp(instr);
        break;
      }
      }
      break;
    }
    case OP_RES:
    case OP_RTI:
    default: {
      printf("Unknown opcode: %d", op);
      abort();
      break;
    }
    }
  }
}
