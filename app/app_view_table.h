#ifndef APP_VIEW_TABLE_H_
#define APP_VIEW_TABLE_H_

#include "./app_display.h"
#include "./app_view.h"

class App_View_TableField {
public:
    virtual void render(App_Display* display, uint8_t row, uint8_t col, uint8_t selectedRow, uint8_t selectedCol) = 0;

    virtual uint8_t update(UiKeys* keys, App_Display* display)
    {
        return VIEW_NONE;
    }

    virtual bool isSelectable(uint8_t row, uint8_t col)
    {
        return false;
    }
};

class App_View_Table : App_View {
protected:
    const uint8_t ROW_COUNT;
    const uint8_t COL_COUNT;
    App_View_TableField** fields;

    App_View_TableField* getSelectedField()
    {
        return fields[selectedRow * COL_COUNT + selectedCol];
    }

    void selectNextRow(int8_t direction)
    {
        uint8_t start = selectedRow;
        do {
            selectedRow = (selectedRow + ROW_COUNT + direction) % ROW_COUNT;
            if (getSelectedField()->isSelectable(selectedRow, selectedCol)) {
                break;
            }
        } while (selectedRow != start);
    }

    void selectNextCol(int8_t direction)
    {
        uint8_t start = selectedCol;
        do {
            selectedCol = (selectedCol + COL_COUNT + direction) % COL_COUNT;
            if (getSelectedField()->isSelectable(selectedRow, selectedCol)) {
                break;
            }
        } while (selectedCol != start);
    }

public:
    uint8_t selectedRow = 0;
    uint8_t selectedCol = 0;

    App_View_Table(App_View_TableField** _fields, uint8_t _row, uint8_t _col)
        : ROW_COUNT(_row)
        , COL_COUNT(_col)
        , fields(_fields)
    {
        // initSelection();
    }

    void initSelection()
    {
        for (uint8_t row = 0; row < ROW_COUNT; row++) {
            for (uint8_t col = 0; col < COL_COUNT; col++) {
                App_View_TableField* field = fields[row * COL_COUNT + col];
                if (field == NULL) {
                    printf("isnull %d %d\n", row, col);
                }
                if (field != NULL && field->isSelectable(row, col)) {
                    selectedRow = row;
                    selectedCol = col;
                    printf("selected %d %d\n", selectedRow, selectedCol);
                    return;
                }
            }
        }
        printf("no slectable field!!!!!\n");
    }

    virtual void initDisplay(App_Display* display)
    {
        strcpy(display->text, "");
    }

    virtual void endRow(App_Display* display, uint16_t row)
    {
        strcat(display->text, "\n");
    }

    void render(App_Display* display)
    {
        initDisplay(display);
        for (uint8_t row = 0; row < ROW_COUNT; row++) {
            // here would come if visible row
            for (uint8_t col = 0; col < COL_COUNT; col++) {
                // here would come if visible col
                App_View_TableField* field = fields[row * COL_COUNT + col];
                if (field != NULL) {
                    field->render(display, row, col, selectedRow, selectedCol);
                }
            }
            endRow(display, row);
        }
    }

    uint8_t update(UiKeys* keys, App_Display* display)
    {
        if (keys->A) {
            return getSelectedField()->update(keys, display);
        } else {
            if (keys->Up) {
                selectNextRow(-1);
            } else if (keys->Down) {
                selectNextRow(+1);
            } else if (keys->Left) {
                selectNextCol(-1);
            } else if (keys->Right) {
                selectNextCol(+1);
            }
            render(display);
            return VIEW_CHANGED;
        }
        return VIEW_NONE;
    }
};

// template <uint8_t VISIBLE_ROW, uint8_t VISIBLE_COL, uint8_t TOTAL_COL>
// class App_View_Table : App_View {
// public:
//     uint16_t startPos = 0;
//     uint16_t cursor = 0;
//     uint16_t TOTAL_ROW = 0;
//     uint16_t TOTAL_CELL = 0;

//     App_View_Table(uint16_t totalRow)
//         : TOTAL_ROW(totalRow)
//         , TOTAL_CELL(totalRow * TOTAL_COL)
//     {
//     }

//     virtual void renderCell(App_Display* display, uint16_t pos, uint16_t row, uint8_t col) = 0;

//     virtual void initDisplay(App_Display* display)
//     {
//         strcpy(display->text, "");
//     }

//     virtual void colSeparator(App_Display* display)
//     {
//         strcat(display->text, " ");
//     }

//     virtual void endRow(App_Display* display, uint16_t row)
//     {
//         strcat(display->text, "\n");
//     }

//     virtual void startRow(App_Display* display, uint16_t row)
//     {
//     }

//     void render(App_Display* display)
//     {
//         initDisplay(display);
//         for (uint16_t row = 0, n = 0; row < VISIBLE_ROW; row++, n += TOTAL_COL - VISIBLE_COL) {
//             startRow(display, row);
//             for (uint8_t col = 0; col < VISIBLE_COL; col++, n++) {
//                 if (col != 0) {
//                     colSeparator(display);
//                 }
//                 renderCell(display, n + startPos, row, col);
//             }
//             endRow(display, row);
//         }
//     }

//     uint8_t update(UiKeys* keys, App_Display* display)
//     {
//         if (keys->Up || keys->Down || keys->Right || keys->Left) {
//             if (keys->Up) {
//                 if ((int16_t)cursor >= TOTAL_COL) {
//                     cursor -= TOTAL_COL;
//                     if (cursor < startPos) {
//                         startPos -= TOTAL_COL;
//                     }
//                 }
//             } else if (keys->Down) {
//                 if (cursor + TOTAL_COL < TOTAL_CELL) {
//                     cursor += TOTAL_COL;
//                     if (cursor >= startPos + (TOTAL_COL * VISIBLE_ROW)) {
//                         startPos += TOTAL_COL;
//                     }
//                 }
//             } else if (keys->Right) {
//                 if (cursor % TOTAL_COL < TOTAL_COL - 1) {
//                     cursor++;
//                     if ((cursor % TOTAL_COL) - (startPos % TOTAL_COL) == VISIBLE_COL) {
//                         startPos++;
//                     }
//                 }
//             } else if (keys->Left) {
//                 if (cursor % TOTAL_COL > 0) {
//                     cursor--;
//                     if (cursor % TOTAL_COL < startPos % TOTAL_COL) {
//                         startPos--;
//                     }
//                 }
//             }
//             // SDL_Log("startPos %d,%d cursor %d,%d", startPos, startPos % TOTAL_COL, cursor, cursor % TOTAL_COL);
//             // SDL_Log("%d == %d", (cursor % TOTAL_COL) - (startPos % TOTAL_COL), VISIBLE_COL);
//             render(display);
//             return VIEW_CHANGED;
//         }
//         return VIEW_NONE;
//     }
// };

#endif