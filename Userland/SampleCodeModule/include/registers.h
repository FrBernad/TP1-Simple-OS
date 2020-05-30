#ifndef REGISTERS_H
#define REGISTERS_H

#include <stdint.h>

#define REGISTERS 17
#define REG_NAME 5

typedef struct {
      uint64_t data[REGISTERS];
      char name[REGISTERS][REG_NAME];
} t_registers;

#endif
