#ifndef RTC_TIME_H
#define RTC_TIME_H

#include <stdint.h>

typedef enum {
      SECONDS = 0x00,
      MINUTES = 0x02,
      HOURS = 0x04
} t_timeInfo;

uint8_t getDecimalTimeInfo(uint8_t info);
uint8_t getBSDTimeInfo(uint8_t info);

#endif