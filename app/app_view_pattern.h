#ifndef APP_VIEW_PATTERN_H_
#define APP_VIEW_PATTERN_H_

#include "./app_display.h"
#include "./app_view.h"

#define VIEW_PATTERN_ROW 1 //2 + MAX_STEPS_IN_PATTERN
#define VIEW_PATTERN_COL 5

class App_View_PatternNumber : public App_View_TableField {
protected:
    uint8_t* currentPatternId;

public:
    App_View_PatternNumber(uint8_t* _currentPatternId)
        : currentPatternId(_currentPatternId)
    {
    }

    bool isSelectable(uint8_t row, uint8_t col) override
    {
        return col != 0;
    }

    void render(App_Display* display, uint8_t row, uint8_t col, uint8_t selectedRow, uint8_t selectedCol)
    {
        if (col == 0) {
            strcat(display->text, "PAT");
        } else {
            if (selectedRow == row && selectedCol == col) {
                display->setCursor(3, 1);
            }
            sprintf(display->text + strlen(display->text), " %03d", *currentPatternId + 1);
        }
    }

    uint8_t update(UiKeys* keys, App_Display* display, uint8_t row, uint8_t col)
    {
        int8_t direction = 0;
        if (keys->Right) {
            direction = 1;
        } else if (keys->Left) {
            direction = -1;
        } else if (keys->Up) {
            direction = 10;
        } else if (keys->Down) {
            direction = -10;
        }
        *currentPatternId = (*currentPatternId + PATTERN_COUNT + direction) % PATTERN_COUNT;
        return VIEW_CHANGED;
    }
};

class App_View_Pattern : public App_View_Table {
protected:
    App_Patterns* patterns;
    uint8_t currentPatternId = 0;

    App_View_PatternNumber patNumberField;

    App_View_TableField* fields[VIEW_PATTERN_ROW * VIEW_PATTERN_COL] = {
        // clang-format off
        &patNumberField, &patNumberField, NULL, NULL, NULL,
        // clang-format on
    };

public:
    App_View_Pattern(App_Patterns* _patterns)
        : App_View_Table(fields, VIEW_PATTERN_ROW, VIEW_PATTERN_COL)
        , patterns(_patterns)
        , patNumberField(&currentPatternId)
    {
        initSelection();
    }

    void initDisplay(App_Display* display)
    {
        display->useColoredLabel();
        App_View_Table::initDisplay(display);
    }
};

#endif