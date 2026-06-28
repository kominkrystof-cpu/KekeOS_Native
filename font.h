#ifndef FONT_H
#define FONT_H

#include <stdint.h>

// Rozměry jednoho znaku v pixelech
#define FONT_WIDTH   8
#define FONT_HEIGHT  16

// Počet znaků v tabulce (ASCII 0-127)
#define FONT_CHARS   128

// Deklarace bitmapy – definice je v font.c
extern const uint8_t font_bitmap[FONT_CHARS][FONT_HEIGHT];

#endif