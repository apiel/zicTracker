#ifndef UI_FONT_H_
#define UI_FONT_H_

#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "fontData.h"

#define TextWhite 65535
#define TextRed ((255 >> 3) << 11) + ((0 >> 2) << 5) + (0 >> 3)
#define TextBlue ((0 >> 3) << 11) + ((0 >> 2) << 5) + (255 >> 3)

void setPixel(SDL_Surface* surface, int32_t x, int32_t y, uint16_t c)
{
    *((uint16_t*)surface->pixels + ((x) + (y)*surface->w)) = c;
}

void drawChar(SDL_Surface* surface, int32_t* x, int32_t* y, char ch, uint16_t fc, uint16_t olc)
{
    int32_t i, j;
    uint8_t* charSprite;
    if (ch == '\n') {
        *x = 0;
        *y += 8;
    } else if (*y < surface->h - 1) {
        charSprite = ch * 8 + fontData;
        // Draw charSprite as monochrome 8*8 image using given color
        for (i = 0; i < 8; i++) {
            for (j = 7; j >= 0; j--) {
                if ((charSprite[i] >> j) & 1) {
                    setPixel(surface, *x + (7 - j), *y + i, fc);
                }
                // else if (isOutlinePixel(charSprite, 7 - j, i)) {
                //     setPixel(surface, *x + (7 - j), *y + i, olc);
                // }
            }
        }
        *x += 8;
    }
}

void draw_string(SDL_Surface* surface, const char* text, int32_t x, int32_t y, Uint8 size = 1, Uint32 color = 255)
{
    unsigned long i, max = strlen(text) + 1;
    for (i = 0; i < max; i++) {
        drawChar(surface, &x, &y, text[i], TextWhite, 0);
    }
}


// static int32_t isOutlinePixel(uint8_t* charfont, int32_t x, int32_t y)
// {
//     int32_t xis0 = !x, xis7 = x == 7, yis0 = !y, yis7 = y == 7;

//     if (xis0) {
//         if (yis0) {
//             return !(*charfont & 0x80) && ((*charfont & 0x40) || (charfont[1] & 0x80) || (charfont[1] & 0x40));
//         } else if (yis7) {
//             return !(charfont[7] & 0x80) && ((charfont[7] & 0x40) || (charfont[6] & 0x80) || (charfont[6] & 0x40));
//         } else {
//             return !(charfont[y] & 0x80) && ((charfont[y - 1] & 0x80) || (charfont[y - 1] & 0x40) || (charfont[y] & 0x40) || (charfont[y + 1] & 0x80) || (charfont[y + 1] & 0x40));
//         }
//     } else if (xis7) {
//         if (yis0) {
//             return !(*charfont & 0x01) && ((*charfont & 0x02) || (charfont[1] & 0x01) || (charfont[1] & 0x02));
//         } else if (yis7) {
//             return !(charfont[7] & 0x01) && ((charfont[7] & 0x02) || (charfont[6] & 0x01) || (charfont[6] & 0x02));
//         } else {
//             return !(charfont[y] & 0x01) && ((charfont[y - 1] & 0x01) || (charfont[y - 1] & 0x02) || (charfont[y] & 0x02) || (charfont[y + 1] & 0x01) || (charfont[y + 1] & 0x02));
//         }
//     } else {
//         int32_t b = 1 << (7 - x);
//         if (yis0) {
//             return !(*charfont & b) && ((*charfont & (b << 1)) || (*charfont & (b >> 1)) || (charfont[1] & (b << 1)) || (charfont[1] & b) || (charfont[1] & (b >> 1)));
//         } else if (yis7) {
//             return !(charfont[7] & b) && ((charfont[7] & (b << 1)) || (charfont[7] & (b >> 1)) || (charfont[6] & (b << 1)) || (charfont[6] & b) || (charfont[6] & (b >> 1)));
//         } else {
//             return !(charfont[y] & b) && ((charfont[y] & (b << 1)) || (charfont[y] & (b >> 1)) || (charfont[y - 1] & (b << 1)) || (charfont[y - 1] & b) || (charfont[y - 1] & (b >> 1)) || (charfont[y + 1] & (b << 1)) || (charfont[y + 1] & b) || (charfont[y + 1] & (b >> 1)));
//         }
//     }
// }
#endif