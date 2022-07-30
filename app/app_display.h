#ifndef APP_DISPLAY_H_
#define APP_DISPLAY_H_

#include "./app_display_base.h"

#ifdef FONT_FILE
#include FONT_FILE
#else
#include "./fonts/fontData5x7.h"
#endif

#ifndef LINE_SPACING
#define LINE_SPACING 2
#endif

#define UI_COLOR_BG 0, 0, 0
#define UI_COLOR_FONT 0xFF, 0xFF, 0xFF
#define UI_COLOR_LABEL 150, 150, 150
#define UI_COLOR_HEADER 100, 100, 100
#define UI_COLOR_PLAY 122, 255, 0
#define UI_COLOR_STAR 255, 255, 0
#define UI_COLOR_CURSOR 0, 122, 255

class App_Display : public App_Display_Base {
protected:
    uint16_t xScreen = 0;
    uint16_t yScreen = 0;

    // Could be boolean and use it to draw char process...
    virtual void drawPixel(int16_t x, int16_t y) = 0;
    virtual void drawCursor(int16_t x, int16_t y) = 0;
    virtual void setColor(uint8_t r, uint8_t g, uint8_t b) = 0;

    void drawChar(unsigned char chr, uint16_t x, uint16_t y)
    {
        const unsigned char* ptr = fontData + (chr - FONT_ASCII_START) * FONT_H;

        for (int i = 0; i < FONT_H; i++, ptr++) {
            for (int j = 8 - FONT_W; j < 8; j++) { // Support only 8 bits font
                if ((*ptr >> j) & 1) {
                    drawPixel(x + (8 - j), y + i);
                }
            }
        }
    }

    uint8_t getRow(uint16_t y)
    {
        return (y / ((FONT_H + LINE_SPACING))) + startRow;
    }

    void resetColor(uint16_t y)
    {
        uint8_t row = getRow(y);
        if (coloredHeader[0] == row || coloredHeader[1] == row) {
            setColor(UI_COLOR_HEADER);
        } else {
            setColor(UI_COLOR_FONT);
        }
    }

public:
    virtual void clearScreen() = 0;

    void reset() override
    {
        clearScreen();
        App_Display_Base::reset();
    }

    void drawText()
    {
        uint16_t x = xScreen;
        uint16_t y = yScreen;
        uint16_t orig_x = x;
        const char* txt = text;
        resetColor(y);

        while (*txt) {
            if (*txt == '\n') {
                x = orig_x;
                y += (FONT_H + LINE_SPACING);
                resetColor(y);
            } else {
                if (isColoredLabel()
                    && x == orig_x + (coloredLabel * FONT_W)
                    && getRow(y) >= coloredLabelFrom) {
                    setColor(UI_COLOR_LABEL);
                }
                if (*txt == '>') {
                    setColor(UI_COLOR_PLAY);
                } else if (*txt == '*') {
                    setColor(UI_COLOR_STAR);
                } else if (*txt == ' ' || *txt == '\n') {
                    resetColor(y);
                }

                if (cursorLen && txt >= cursorPos && txt < cursorPos + cursorLen) {
                    drawCursor(x, y);
                }

                drawChar(*txt, x, y);
                x += FONT_W;
            }
            txt++;
        }
    }
};

#endif
