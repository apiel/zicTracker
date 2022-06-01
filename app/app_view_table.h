#ifndef APP_VIEW_TABLE_H_
#define APP_VIEW_TABLE_H_

#include "./app_view.h"

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

    virtual void renderPos(char* display, uint16_t pos) = 0;

    void initDisplay(char* display)
    {
        strcpy(display, "");
    }

    void colSeparator(char* display)
    {
        strcat(display, " ");
    }

    void rowSeparator(char* display)
    {
        strcat(display, "\n");
    }

    void render(char* display)
    {
        initDisplay(display);
        for (uint8_t row = 0, n = 0; row < VISIBLE_ROW; row++, n += TOTAL_COL - VISIBLE_COL) {
            for (uint8_t col = 0; col < VISIBLE_COL; col++, n++) {
                if (col != 0) {
                    colSeparator(display);
                }
                renderPos(display, n + startPos);
            }
            rowSeparator(display);
        }
    }

    uint8_t update(UiKeys* keys, char* display)
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