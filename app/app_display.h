#ifndef APP_DISPLAY_H_
#define APP_DISPLAY_H_

#include "./app_renderer.h"

#ifdef FONT_FILE
#include FONT_FILE
#else
#include "./fonts/fontData5x7.h"
#endif

#ifndef LINE_SPACING
#define LINE_SPACING 2
#endif

// Colors could be set per view...

#define UI_COLOR_BG 0, 0, 0
#define UI_COLOR_FONT 0xFF, 0xFF, 0xFF
#define UI_COLOR_LABEL 150, 150, 150
#define UI_COLOR_HEADER 100, 100, 100
#define UI_COLOR_PLAY 122, 255, 0
// #define UI_COLOR_STAR 255, 128, 0
#define UI_COLOR_STAR 255, 255, 0
#define UI_COLOR_CURSOR 0, 128, 255
// #define UI_COLOR_SIGN 0, 255, 255
// #define UI_COLOR_SIGN 255, 128, 0
#define UI_COLOR_SIGN 190, 190, 190

class App_Display : public App_Renderer {
protected:
    uint16_t xScreen = 0;
    uint16_t yScreen = 0;
    uint8_t charCount = 0;

    // Could be boolean and use it to draw char process...
    virtual void drawPixel(int16_t x, int16_t y) = 0;
    virtual void drawCursor(int16_t x, int16_t y) = 0;
    virtual void setColor(uint8_t r, uint8_t g, uint8_t b) = 0;

    virtual void drawChar(unsigned char chr, uint16_t x, uint16_t y)
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

    uint16_t levelSpecialChar[6] = {
        *(uint16_t*)("⠠" + 1),
        *(uint16_t*)("⠤" + 1),
        *(uint16_t*)("⠴" + 1),
        *(uint16_t*)("⠶" + 1),
        *(uint16_t*)("⠾" + 1),
        *(uint16_t*)("⠿" + 1),
    };

    virtual void drawSpecialChar(uint16_t val, uint16_t x, uint16_t y)
    {
        uint8_t level = 0;
        for (; level < 6; level++) {
            if (val == levelSpecialChar[level]) {
                break;
            }
        }

        setColor(UI_COLOR_HEADER);
        if (level >= 5) {
            drawPixel(x + 5, y);
            drawPixel(x + 6, y);
            drawPixel(x + 5, y + 1);
            drawPixel(x + 6, y + 1);
        }

        if (level >= 4) {
            drawPixel(x + 2, y);
            drawPixel(x + 3, y);
            drawPixel(x + 2, y + 1);
            drawPixel(x + 3, y + 1);
        }

        if (level >= 3) {

            drawPixel(x + 5, y + 4);
            drawPixel(x + 6, y + 4);
            drawPixel(x + 5, y + 3);
            drawPixel(x + 6, y + 3);
        }

        if (level >= 2) {
            drawPixel(x + 2, y + 3);
            drawPixel(x + 3, y + 3);
            drawPixel(x + 2, y + 4);
            drawPixel(x + 3, y + 4);
        }

        if (level >= 1) {
            drawPixel(x + 5, y + 7);
            drawPixel(x + 6, y + 7);
            drawPixel(x + 5, y + 6);
            drawPixel(x + 6, y + 6);
        }

        drawPixel(x + 2, y + 6);
        drawPixel(x + 3, y + 6);
        drawPixel(x + 2, y + 7);
        drawPixel(x + 3, y + 7);
        setColor(UI_COLOR_FONT);
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
        charCount = 0;
    }

public:
    virtual void clearScreen() = 0;

    void reset() override
    {
        clearScreen();
        App_Renderer::reset();
    }

    virtual void drawText()
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
                } else if (*txt == '+' || *txt == '-') {
                    setColor(UI_COLOR_SIGN);
                } else if (*txt == ' ' || *txt == '\n') {
                    resetColor(y);
                } else if (firstLetter) {
                    if (charCount > 0) {
                        setColor(UI_COLOR_LABEL);
                    }
                    charCount++;
                }

                if (cursorLen && txt >= cursorPos && txt < cursorPos + cursorLen) {
                    drawCursor(x, y);
                }

                if (*txt == -30) {
                    drawSpecialChar(*(uint16_t*)(txt + 1), x, y);
                    txt += 2;
                } else {
                    drawChar(*txt, x, y);
                }
                x += FONT_W;
            }
            txt++;
        }
    }
};

#endif
