#ifndef UI_FONT_H_
#define UI_FONT_H_

#include <SDL2/SDL.h>

#include "../app/app_display.h"
#include "fontData.h"

#define FONT_H 8
#define FONT_W 8
#define LINE_SPACING 8

#define COLOR_SYMBOL '~'

Uint32 fontColor = 0;

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

void init_default_string_color(SDL_Surface* surface)
{
    fontColor = SDL_MapRGB(surface->format, 0xFF, 0xFF, 0xFF);
}

void draw_string(SDL_Surface* surface, App_Display* display, Uint16 x, Uint16 y, Uint8 size = 1)
{
    Uint16 orig_x = x;
    const char* text = display->text;
    while (*text) {
        if (*text == '\n') {
            x = orig_x;
            y += (FONT_H + LINE_SPACING) * size;
        } else {
            if (*text == '>') { // Play symbol
                fontColor = SDL_MapRGB(surface->format, 73, 219, 158);
            } else if (*text == ' ' || *text == '\n') {
                init_default_string_color(surface);
            }

            if (display->cursorLen && text >= display->cursorPos && text < display->cursorPos + display->cursorLen) {
                SDL_Rect r = { x * size - 2, y, FONT_W * size, FONT_H * size };
                SDL_FillRect(surface, &r, SDL_MapRGB(surface->format, 39, 69, 94));
            }

            draw_char(surface, *text, x, y, size);
            x += FONT_W; // * size;
        }
        text++;
    }
}

#endif