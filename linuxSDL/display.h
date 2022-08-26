#ifndef UI_DISPLAY_H_
#define UI_DISPLAY_H_

#include <SDL2/SDL.h>

// #define FONT_SIZE 3

#define FONT_FILE "./fonts/fontData8x8.h"
#define FONT_SIZE 2
#define LINE_SPACING 8

#include "../app/app_display.h"

// rg351
#define SCREEN_W 480
#define SCREEN_H 320

// #define SCREEN_W 320
// #define SCREEN_H 240

class UI_Display : public App_Display {
protected:
    SDL_Surface* surface = NULL;

    Uint32 fontColor = 0;

    void drawPixel(int16_t x, int16_t y)
    {
        if (x < 0 || x * FONT_SIZE >= SCREEN_W || y < 0 || y * FONT_SIZE >= SCREEN_H) {
            return;
        }

        // #if FONT_SIZE == 1
        // should tryy to just draw a pixel....
        // #else
        SDL_Rect r = { x * FONT_SIZE, y * FONT_SIZE, FONT_SIZE, FONT_SIZE };
        SDL_FillRect(surface, &r, fontColor);
    }

    void drawCursor(int16_t x, int16_t y)
    {
        SDL_Rect r = { x * FONT_SIZE, y * FONT_SIZE, FONT_W * FONT_SIZE, FONT_H * FONT_SIZE };
        SDL_FillRect(surface, &r, rgb(UI_COLOR_CURSOR));
    }

    Uint32 rgb(uint8_t r, uint8_t g, uint8_t b)
    {
        return SDL_MapRGB(surface->format, r, g, b);
    }

    void setColor(uint8_t r, uint8_t g, uint8_t b)
    {
        fontColor = rgb(r, g, b);
    }

public:
    UI_Display()
    {
        xScreen = 4;
        yScreen = 5;
    }

    // Dont forget to call at start, else crash
    void init(SDL_Surface* _surface)
    {
        surface = _surface;
    }

    void clearScreen()
    {
        SDL_FillRect(surface, NULL, rgb(UI_COLOR_BG));
    }

    bool ready() override
    {
        return surface != NULL;
    }
};

#endif