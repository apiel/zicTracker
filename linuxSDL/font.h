#ifndef UI_FONT_H_
#define UI_FONT_H_

#include <SDL2/SDL.h>

#include "fontData.h"

#define FONT_H 8
#define FONT_W 8
#define LINE_SPACING 4

#define COLOR_SYMBOL '~'

Uint32 fontColor = 0;
Uint32 bgColor = 0;
bool readColor = false;

void setColor(const SDL_PixelFormat* format, unsigned char color)
{
    switch (color) {
    case '1':
        fontColor = SDL_MapRGB(format, 73, 219, 158);
        break;

    case 'b':
        bgColor = SDL_MapRGB(format, 39, 69, 94);
        break;

    default:
        fontColor = SDL_MapRGB(format, 0xFF, 0xFF, 0xFF);
        bgColor = 0;
        break;
    }
}

void draw_char(SDL_Surface* surface, unsigned char symbol, Uint16 x, Uint16 y, Uint8 size)
{
    x += (FONT_H - 1) * 1;
    if (symbol > 127) {
        symbol -= 128;
    }
    const unsigned char* ptr = fontData + symbol * FONT_H;

    for (int i = 0, ys = 0; i < FONT_W; i++, ptr++, ys += 1) {
        for (int col = FONT_H - FONT_W, xs = x - col; col < FONT_H; col++, xs -= 1) {
            if ((*ptr & 1 << col) && y + ys < surface->h && xs < surface->w) {
                SDL_Rect r = { xs * size, y + ys * size, size, size };
                SDL_FillRect(surface, &r, fontColor);
            }
        }
    }
}

void draw_string(SDL_Surface* surface, const char* text, Uint16 x, Uint16 y, Uint8 size = 1, Uint32 color = 255)
{
    if (color != 255) {
        setColor(surface->format, color);
    }
    Uint16 orig_x = x;
    while (*text) {
        if (readColor) {
            readColor = false;
            if (*text != COLOR_SYMBOL) {
                setColor(surface->format, *text);
            }
        } else if (*text == COLOR_SYMBOL) {
            readColor = true;
        } else if (*text == '\n') {
            x = orig_x;
            y += (FONT_H + LINE_SPACING) * size;
        } else {
            if (bgColor) {
                SDL_Rect r = { x * size, y, FONT_H * size, FONT_W * size };
                SDL_FillRect(surface, &r, bgColor);
            }
            draw_char(surface, *text, x, y, size);
            x += FONT_W; // * size;
        }
        text++;
    }
}

#endif