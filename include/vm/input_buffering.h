#ifndef VM_INPUT_BUFFERING_H__
#define VM_INPUT_BUFFERING_H__

#include <stdint.h>
#include <sys/termios.h>

void disable_input_buffering();
void restore_input_buffering();
uint16_t check_key();
void handle_interrupt(int signal);

#endif
