#ifndef APP_VIEW_TABLE_H_
#define APP_VIEW_TABLE_H_

#include "./app_view.h"

template <uint8_t VISIBLE_ROW, uint8_t VISIBLE_COL, uint8_t TOTAL_COL>
class App_View_Table : App_View {
public:
    uint16_t pos = 0;
    uint16_t cursor = 0;

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
                renderPos(display, n + pos);
            }
            rowSeparator(display);
        }
    }

    // uint8_t update(UiKeys* keys, char* display)
    // {
    //     if (keys->Up || keys->Down || keys->Right || keys->Left) {
    //         if (keys->Up) {
    //             if ((int16_t)cursor - 7 >= 0) {
    //                 cursor -= 7;
    //                 if (cursor < pos) {
    //                     pos -= 7;
    //                 }
    //             }
    //         } else if (keys->Down) {
    //             if (cursor + 7 < 8 * 7) {
    //                 cursor += 7;
    //                 // FIXME fixme
    //                 if (cursor > pos + (7 * APP_VIEW_TRACK_LOOP_ROW)) {
    //                     pos += 7;
    //                 }
    //             }
    //         } else if (keys->Right) {
    //             if (cursor % 7 < 6) {
    //                 cursor++;
    //                 if (cursor % 7 > 7 - APP_VIEW_TRACK_LOOP_COL) {
    //                     pos++;
    //                 }
    //             }
    //         } else if (keys->Left) {
    //             if (cursor % 7 != 0) {
    //                 cursor--;
    //                 if (cursor % 7 < pos % 7) {
    //                     pos--;
    //                 }
    //             }
    //         }
    //         SDL_Log("pos %d,%d cursor %d,%d", pos, pos % 7, cursor, cursor % 7);
    //         render(display);
    //         return VIEW_CHANGED;
    //     }
    //     return VIEW_NONE;
    // }
};

#endif