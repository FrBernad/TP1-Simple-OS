#ifndef UTILS_H
#define UTILS_H

#include <buffer.h>
#include <stdint.h>
#include <systemCalls.h>
#include <stringLib.h>

#define IS_LETTER(c) (c >= 'a' && c <= 'z' ? 1 : 0)
#define IS_DIGIT(c) (c >= '0' && c <= '9' ? 1 : 0)
#define IS_NUMBER(c) (c >= '0' && c <= '9' ? 1 : 0)
#define ABS(c) (c >= 0 ? c : c * -1)

uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base);
uint8_t BSDToInt(uint8_t num);
uint64_t strToInt(char *str, int *error);
uint64_t pow(uint64_t x, uint64_t y);
char * strtok(char *string, char *result, const char delim);
uint8_t stringcmp(char *str1, char *str2);
int ticksElapsed();
void cleanBuffer(t_buffer *buffer);
void cleanString(char *str);
int strlen(char *str);

#endif