#include "fb_text.h"
#include "font.h"
#include <stdbool.h>

// ── Fixní konstanty z GRUB (1024x768x32) ─────────────────────
#define FB_WIDTH   1024
#define FB_HEIGHT  768
#define FB_PITCH   4096  // 1024 * 4 bajty

// ── Interní stav konzole ──────────────────────────────────────
static uint32_t cursor_col = 0;
static uint32_t cursor_row = 0;
static uint32_t cols = 128;  // 1024 / 8
static uint32_t rows = 48;   // 768 / 16

#define DEFAULT_FG  0x00FFFFFF
#define DEFAULT_BG  0xFFFFFFFF

extern void put_pixel(uint32_t x, uint32_t y, uint32_t color);

void fb_text_init(void) {
    cursor_col = 0;
    cursor_row = 0;
}

void fb_put_char(uint32_t x, uint32_t y, char c, uint32_t fg, uint32_t bg) {
    if ((uint8_t)c >= FONT_CHARS) c = '?';
    bool draw_bg = (bg != 0xFFFFFFFF);

    for (uint32_t row = 0; row < FONT_HEIGHT; row++) {
        uint8_t byte = font_bitmap[(uint8_t)c - 1][row];
        for (uint32_t col = 0; col < FONT_WIDTH; col++) {
            if (byte & (0x80 >> col)) {
                put_pixel(x + col, y + row, fg);
            } else if (draw_bg) {
                put_pixel(x + col, y + row, bg);
            }
        }
    }
}

void fb_print(uint32_t x, uint32_t y, const char* str, uint32_t fg, uint32_t bg) {
    uint32_t cx = x;
    uint32_t cy = y;

    while (*str) {
        if (*str == '\n') {
            cx = x;
            cy += FONT_HEIGHT;
        } else {
            fb_put_char(cx, cy, *str, fg, bg);
            cx += FONT_WIDTH;
        }
        str++;
    }
}

void fb_putc(char c, uint32_t fg) {
    if (c == '\n') {
        cursor_col = 0;
        cursor_row++;
        if (cursor_row >= rows) {
            fb_scroll();
        }
        return;
    }

    if (c == '\b') {
        if (cursor_col > 0) {
            cursor_col--;
            fb_put_char(cursor_col * FONT_WIDTH, cursor_row * FONT_HEIGHT, ' ', fg, 0x000000);
        }
        return;
    }

    fb_put_char(cursor_col * FONT_WIDTH, cursor_row * FONT_HEIGHT, c, fg, 0x000000);
    cursor_col++;

    if (cursor_col >= cols) {
        cursor_col = 0;
        cursor_row++;
        if (cursor_row >= rows) {
            fb_scroll();
        }
    }
}

void fb_puts(const char* str, uint32_t fg) {
    while (*str) {
        if (*str == '\n') {
            cursor_col = 0;
            cursor_row++;
        } else if (*str == '\r') {
            cursor_col = 0;
        } else {
            fb_put_char(cursor_col * FONT_WIDTH, cursor_row * FONT_HEIGHT, *str, fg, DEFAULT_BG);
            cursor_col++;

            if (cursor_col >= cols) {
                cursor_col = 0;
                cursor_row++;
            }
        }

        if (cursor_row >= rows) {
            fb_scroll();
            cursor_row = rows - 1;
        }

        str++;
    }
}

void fb_scroll(void) {
    // Zjednodušený scrolling - pouze posun kurzoru nahoru
    // Plný pixelový scrolling by vyžadoval přímý přístup k framebufferu
    cursor_row = rows - 1;
}

uint32_t fb_get_addr(void) {
    // Vrátí adresu framebufferu (prozatím fixní adresa z GRUB)
    return 0;
}

uint32_t fb_get_pitch(void) {
    // Vrátí pitch (bytes per row)
    return FB_PITCH;
}

void fb_clear(uint32_t color) {
    cursor_col = 0;
    cursor_row = 0;

    // Použijeme put_pixel pro mazání celé obrazovky
    for (uint32_t y = 0; y < FB_HEIGHT; y++) {
        for (uint32_t x = 0; x < FB_WIDTH; x++) {
            put_pixel(x, y, color);
        }
    }
}