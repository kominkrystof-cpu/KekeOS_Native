#ifndef FB_TEXT_H
#define FB_TEXT_H

#include <stdint.h>

// Inicializace textového rendereru
// Musí být zavolána po fb_init()
void fb_text_init(void);

// Vykreslí jeden ASCII znak na pozici (x, y) v pixelech
void fb_put_char(uint32_t x, uint32_t y,
                 char c,
                 uint32_t fg,   // barva popředí 0x00RRGGBB
                 uint32_t bg);  // barva pozadí (0 = průhledné)

// Vykreslí řetězec – automaticky zalomí řádky
void fb_print(uint32_t x, uint32_t y,
              const char* str,
              uint32_t fg, uint32_t bg);

// Tiskne na "konzolovou" pozici (sleduje kurzor automaticky)
void fb_puts(const char* str, uint32_t fg);

// Vykreslí jeden znak na aktuální pozici kurzoru
void fb_putc(char c, uint32_t fg);

// Posune obsah obrazovky o jeden řádek nahoru
void fb_scroll(void);

// Vyčistí obrazovku
void fb_clear(uint32_t color);

#endif