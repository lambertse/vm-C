#ifndef VM_VM_H__
#define VM_VM_H__

#include "vm/define.h"
#include "vm_impl.h"
#include <stdint.h>
#include <stdio.h>

static uint16_t __swap16(uint16_t x);
static void __read_image_file(FILE *file);
static void __read_image(const char *image_path);
uint16_t __swap16(uint16_t x) { return (x << 8) | (x >> 8); }

void __read_image_file(FILE *file) {
  // The first 16 bits of the program file specify the address
  // in memory where the program should start.
  uint16_t origin;
  fread(&origin, sizeof(origin), 1, file);
  origin = __swap16(origin);

  uint16_t max_read_size = MEMORY_MAX - origin;
  uint16_t *pos = vm_mem + origin;
  size_t read = fread(pos, sizeof(uint16_t), max_read_size, file);

  // swap to little endian
  while (read-- > 0) {
    *pos = __swap16(*pos);
    pos++;
  }
}

void __read_image(const char *image_path) {
  FILE *file = fopen(image_path, "rb");
  if (!file)
    return;

  __read_image_file(file);
  fclose(file);
}

#endif
