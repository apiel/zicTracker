#ifndef APP_VIEW_PATTERN_H_
#define APP_VIEW_PATTERN_H_

#include "./app_display.h"
#include "./app_view.h"

#define VIEW_PATTERN_ROW_HEADERS 4
#define VIEW_PATTERN_ROW (VIEW_PATTERN_ROW_HEADERS + MAX_STEPS_IN_PATTERN)
#define VIEW_PATTERN_COL 5

class App_View_PatternHeader : public App_View_TableField {
protected:
    uint8_t* currentPatternId;

    const char* headers[2] = { "PAT ", "LEN" };

    void renderHeader(App_Display* display, uint8_t col)
    {
        strcat(display->text, headers[col]);
    }

public:
    App_View_PatternHeader(uint8_t* _currentPatternId)
        : currentPatternId(_currentPatternId)
    {
    }

    bool isSelectable(uint8_t row, uint8_t col) override
    {
        return row != 0;
    }

    void render(App_Display* display, uint8_t row, uint8_t col, uint8_t selectedRow, uint8_t selectedCol)
    {
        if (row == 0) {
            renderHeader(display, col);
        } else {
            if (selectedRow == row && selectedCol == col) {
                display->setCursor(3);
            }
            switch (col) {
            case 0:
                sprintf(display->text + strlen(display->text), "%3d ", *currentPatternId + 1);
                break;

            case 1:
                sprintf(display->text + strlen(display->text), "%3d", 64);
                break;
            }
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

class App_View_PatternStepHeader : public App_View_TableField {
protected:
    const char* headers[5] = { "STP ", "I ", "NOT ", "VEL ", "SLID" };

public:
    bool isSelectable(uint8_t row, uint8_t col) override
    {
        return false;
    }

    void render(App_Display* display, uint8_t row, uint8_t col, uint8_t selectedRow, uint8_t selectedCol)
    {
        strcat(display->text, headers[col]);
    }
};

class App_View_PatternStep : public App_View_TableField {
protected:
    App_Patterns* patterns;
    uint8_t* currentPatternId;

    Zic_Seq_Pattern* getPattern()
    {
        return &patterns->patterns[*currentPatternId];
    }

public:
    App_View_PatternStep(App_Patterns* _patterns, uint8_t* _currentPatternId)
        : patterns(_patterns)
        , currentPatternId(_currentPatternId)
    {
    }

    bool isSelectable(uint8_t row, uint8_t col) override
    {
        return col != 0;
    }

    void render(App_Display* display, uint8_t row, uint8_t col, uint8_t selectedRow, uint8_t selectedCol)
    {
        if (selectedRow == row && selectedCol == col) {
            display->setCursor(3);
        }
        uint8_t stepPos = row - VIEW_PATTERN_ROW_HEADERS;
        Zic_Seq_Step* step = &getPattern()->steps[stepPos];
        switch (col) {
        case 0:
            sprintf(display->text + strlen(display->text), "%3d ", stepPos + 1);
            break;

        case 1:
            sprintf(display->text + strlen(display->text), "%c ", step->instrument + 'A');
            break;

        case 2:
            if (step->note == 0) {
                strcat(display->text, "--- ");
            } else {
                sprintf(display->text + strlen(display->text), "%s%d ", Zic::getNoteDash(step->note), Zic::getNoteOctave(step->note));
            }
            break;

        case 3:
            sprintf(display->text + strlen(display->text), "%3d ", step->velocity);
            break;

        case 4:
            sprintf(display->text + strlen(display->text), "%s", step->slide ? "ON " : "OFF");
            break;
        }
    }

    uint8_t update(UiKeys* keys, App_Display* display, uint8_t row, uint8_t col)
    {
        // int8_t direction = 0;
        // if (keys->Right) {
        //     direction = 1;
        // } else if (keys->Left) {
        //     direction = -1;
        // } else if (keys->Up) {
        //     direction = 10;
        // } else if (keys->Down) {
        //     direction = -10;
        // }
        // *currentPatternId = (*currentPatternId + PATTERN_COUNT + direction) % PATTERN_COUNT;
        return VIEW_CHANGED;
    }
};

class App_View_Pattern : public App_View_Table {
protected:
    uint8_t currentPatternId = 0;

    App_View_PatternHeader headerField;
    App_View_PatternStepHeader stepHeaderField;
    App_View_PatternStep stepField;

    App_View_TableField* fields[VIEW_PATTERN_ROW * VIEW_PATTERN_COL] = {
        // clang-format off
        &headerField, &headerField, NULL, NULL, NULL,
        &headerField, &headerField, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL,
        &stepHeaderField, &stepHeaderField, &stepHeaderField, &stepHeaderField, &stepHeaderField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField,
        // clang-format on
    };

public:
    App_View_Pattern(App_Patterns* _patterns)
        : App_View_Table(fields, VIEW_PATTERN_ROW, VIEW_PATTERN_COL)
        , headerField(&currentPatternId)
        , stepField(_patterns, &currentPatternId)
    {
        initSelection();
    }

    void initDisplay(App_Display* display)
    {
        // display->useColoredLabel();
        display->useColoredHeader();
        App_View_Table::initDisplay(display);
    }
};

#endif