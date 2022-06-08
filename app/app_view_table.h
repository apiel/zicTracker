#ifndef APP_VIEW_TABLE_H_
#define APP_VIEW_TABLE_H_

#include "./app_view.h"
#include "./app_display.h"

template <uint8_t VISIBLE_ROW, uint8_t VISIBLE_COL, uint8_t TOTAL_COL>
class App_View_Table : App_View {
public:
    uint16_t startPos = 0;
    uint16_t cursor = 0;
    uint16_t TOTAL_ROW = 0;
    uint16_t TOTAL_CELL = 0;

    App_View_Table(uint16_t totalRow)
        : TOTAL_ROW(totalRow)
        , TOTAL_CELL(totalRow * TOTAL_COL)
    {
    }

    virtual void renderCell(App_Display* display, uint16_t pos, uint16_t row, uint8_t col) = 0;

    virtual void initDisplay(App_Display* display)
    {
        strcpy(display->text, "");
    }

    virtual void colSeparator(App_Display* display)
    {
        strcat(display->text, " ");
    }

    virtual void endRow(App_Display* display, uint16_t row)
    {
        strcat(display->text, "\n");
    }

    virtual void startRow(App_Display* display, uint16_t row)
    {
    }

    void render(App_Display* display)
    {
        initDisplay(display);
        for (uint16_t row = 0, n = 0; row < VISIBLE_ROW; row++, n += TOTAL_COL - VISIBLE_COL) {
            startRow(display, row);
            for (uint8_t col = 0; col < VISIBLE_COL; col++, n++) {
                if (col != 0) {
                    colSeparator(display);
                }
                renderCell(display, n + startPos, row, col);
            }
            endRow(display, row);
        }
    }

    uint8_t update(UiKeys* keys, App_Display* display)
    {
        if (keys->Up || keys->Down || keys->Right || keys->Left) {
            if (keys->Up) {
                if ((int16_t)cursor >= TOTAL_COL) {
                    cursor -= TOTAL_COL;
                    if (cursor < startPos) {
                        startPos -= TOTAL_COL;
                    }
                }
            } else if (keys->Down) {
                if (cursor + TOTAL_COL < TOTAL_CELL) {
                    cursor += TOTAL_COL;
                    if (cursor >= startPos + (TOTAL_COL * VISIBLE_ROW)) {
                        startPos += TOTAL_COL;
                    }
                }
            } else if (keys->Right) {
                if (cursor % TOTAL_COL < TOTAL_COL - 1) {
                    cursor++;
                    if ((cursor % TOTAL_COL) - (startPos % TOTAL_COL) == VISIBLE_COL) {
                        startPos++;
                    }
                }
            } else if (keys->Left) {
                if (cursor % TOTAL_COL > 0) {
                    cursor--;
                    if (cursor % TOTAL_COL < startPos % TOTAL_COL) {
                        startPos--;
                    }
                }
            }
            // SDL_Log("startPos %d,%d cursor %d,%d", startPos, startPos % TOTAL_COL, cursor, cursor % TOTAL_COL);
            // SDL_Log("%d == %d", (cursor % TOTAL_COL) - (startPos % TOTAL_COL), VISIBLE_COL);
            render(display);
            return VIEW_CHANGED;
        }
        return VIEW_NONE;
    }
};

#endif