#ifndef VIDEO_DRIVER_H
#define VIDEO_DRIVER_H

#include <screens.h>
#include <stdint.h>

void writePixel(uint32_t x, uint32_t y, int colour);
void printCharOnScreen(char c, int bgColour, int fontColour, int advance);
void removeCharFromScreen();
void changeLineOnScreen();
void clearScreen();
void scrollDownScreen();
void clearLineOnScreen();
void initVideoDriver(int BGColour, int FontColour);
void changeScreen(int screen);

#endif