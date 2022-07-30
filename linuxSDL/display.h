#ifndef UI_DISPLAY_H_
#define UI_DISPLAY_H_

#if ZIC_SDL2
#include <SDL2/SDL.h>
#else
#include <SDL/SDL.h>
#endif

#include "../app/app_display.h"

#define SCREEN_W 320
#define SCREEN_H 240

class UI_Display : public App_Display {
protected:
    SDL_Surface* surface;

    Uint32 fontColor = 0;
    uint8_t fontSize = 1;

    void drawPixel(int16_t x, int16_t y)
    {
        if (x < 0 || x * fontSize >= SCREEN_W || y < 0 || y * fontSize >= SCREEN_H) {
            return;
        }

        SDL_Rect r = { x * fontSize, y * fontSize, fontSize, fontSize };
        SDL_FillRect(surface, &r, fontColor);
    }

    void drawCursor(int16_t x, int16_t y)
    {
        SDL_Rect r = { x * fontSize, y * fontSize, FONT_W * fontSize, FONT_H * fontSize };
        SDL_FillRect(surface, &r, rgb565(UI_COLOR_CURSOR));
    }

    uint16_t rgb565(uint8_t r, uint8_t g, uint8_t b)
    {
        return SDL_MapRGB(surface->format, r, g, b);
    }

    void setColor(uint8_t r, uint8_t g, uint8_t b)
    {
        fontColor = rgb565(r, g, b);
    }

public:
    // Dont forget to call at start, else crash
    void init(SDL_Surface* _surface)
    {
        surface = _surface;
    }

    void clearScreen()
    {
        SDL_FillRect(surface, NULL, rgb565(UI_COLOR_BG));
    }
};

#endif