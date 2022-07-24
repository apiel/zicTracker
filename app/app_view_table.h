#ifndef APP_VIEW_TABLE_H_
#define APP_VIEW_TABLE_H_

#include "./app_display.h"
#include "./app_view.h"

#ifndef TABLE_VISIBLE_ROWS
#define TABLE_VISIBLE_ROWS 7
#endif

class App_View_TableField {
public:
    virtual void render(App_Display* display, uint8_t row, uint8_t col, uint8_t selectedRow, uint8_t selectedCol) = 0;

    virtual uint8_t update(UiKeys* keys, App_Display* display, uint8_t row, uint8_t col)
    {
        return VIEW_NONE;
    }

    virtual void updateEnd()
    {
    }

    virtual void updateStart()
    {
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
    bool updating = false;

    uint8_t startRow = 0;

    App_View_TableField* getSelectedField()
    {
        return fields[selectedRow * COL_COUNT + selectedCol];
    }

    void selectNextRow(int8_t direction)
    {
        uint8_t start = selectedRow;
        do {
            selectedRow = (selectedRow + ROW_COUNT + direction) % ROW_COUNT;
            if (getSelectedField() && getSelectedField()->isSelectable(selectedRow, selectedCol)) {
                break;
            }
        } while (selectedRow != start);
    }

    void selectNextCol(int8_t direction)
    {
        uint8_t start = selectedCol;
        do {
            selectedCol = (selectedCol + COL_COUNT + direction) % COL_COUNT;
            if (getSelectedField() && getSelectedField()->isSelectable(selectedRow, selectedCol)) {
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
                if (field != NULL && field->isSelectable(row, col)) {
                    selectedRow = row;
                    selectedCol = col;
                    return;
                }
            }
        }
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
        for (uint8_t row = startRow; row < ROW_COUNT && row - startRow < TABLE_VISIBLE_ROWS; row++) {
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
        uint8_t res = VIEW_CHANGED;
        if (keys->A) {
            if (!updating) {
                getSelectedField()->updateStart();
                updating = true;
            }
            res = getSelectedField()->update(keys, display, selectedRow, selectedCol);
        } else {
            if (updating) {
                getSelectedField()->updateEnd();
                updating = false;
            }
            if (keys->Up) {
                selectNextRow(-1);
            } else if (keys->Down) {
                selectNextRow(+1);
            } else if (keys->Left) {
                selectNextCol(-1);
            } else if (keys->Right) {
                selectNextCol(+1);
            }

            if (selectedRow < TABLE_VISIBLE_ROWS * 0.5) {
                startRow = 0;
            } else if (selectedRow > startRow + TABLE_VISIBLE_ROWS - 1) {
                startRow = selectedRow - TABLE_VISIBLE_ROWS + 1;
            } else if (selectedRow < startRow) {
                startRow = selectedRow;
            }
        }
        if (res == VIEW_CHANGED) {
            render(display);
        }
        return res;
    }
};

#endif