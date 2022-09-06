#ifndef APP_VIEW_PATTERN_H_
#define APP_VIEW_PATTERN_H_

#include "./app_renderer.h"
#include "./app_view_table.h"
#include "./app_patterns.h"
#include "./app_view.h"

#define VIEW_PATTERN_ROW_HEADERS 4
#define VIEW_PATTERN_ROW (VIEW_PATTERN_ROW_HEADERS + MAX_STEPS_IN_PATTERN)
// #define VIEW_PATTERN_ROW (VIEW_PATTERN_ROW_HEADERS + 6)
#define VIEW_PATTERN_COL 5

class App_View_PatternHeader : public App_View_TableField {
protected:
    App_Patterns* patterns;
    uint8_t* currentPatternId;

    const char* headers[2] = { "PAT ", "LEN" };

    void renderHeader(App_Renderer* renderer, uint8_t col)
    {
        strcat(renderer->text, headers[col]);
    }

public:
    App_View_PatternHeader(App_Patterns* _patterns, uint8_t* _currentPatternId)
        : patterns(_patterns)
        , currentPatternId(_currentPatternId)
    {
    }

    // TODO by default select pattern base on the last selected track
    // use some event system
    // need to find a way to dispatch event to app
    // and app would popupate the event to each view
    // or use a centralize object to share events

    bool isSelectable(uint8_t row, uint8_t col) override
    {
        return row != 0;
    }

    void render(App_Renderer* renderer, uint8_t row, uint8_t col, uint8_t selectedRow, uint8_t selectedCol)
    {
        if (row == 0) {
            renderHeader(renderer, col);
        } else {
            uint8_t cursorLen[] = { 3, 3, 4, 1 };
            if (selectedRow == row && selectedCol == col) {
                renderer->setCursor(cursorLen[col]);
            }
            switch (col) {
            case 0:
                sprintf(renderer->text + strlen(renderer->text), " %02X ", *currentPatternId + 1);
                break;

            case 1:
                sprintf(renderer->text + strlen(renderer->text), "%3d ", patterns->patterns[*currentPatternId].stepCount);
                break;
            case 2:
                strcat(renderer->text, "SAVE ");
                break;
            case 3:
                strcat(renderer->text, "X ");
                break;
            }
        }
    }

    uint8_t update(UiKeys* keys, App_Renderer* renderer, uint8_t row, uint8_t col)
    {
        if (col == 0) {
            int8_t direction = 0;
            if (keys->Right) {
                direction = 1;
            } else if (keys->Left) {
                direction = -1;
            } else if (keys->Up) {
                direction = col == 0 ? 16 : 10;
            } else if (keys->Down) {
                direction = col == 0 ? -16 : -10;
            }
            *currentPatternId = (*currentPatternId + PATTERN_COUNT + direction) % PATTERN_COUNT;
        } else if (col == 1) {
            uint8_t steps[7] = { 1, 2, 4, 8, 16, 32, 64 };
            uint8_t* stepCount = &patterns->patterns[*currentPatternId].stepCount;
            if (keys->Right) {
                *stepCount = (*stepCount + 1) % MAX_STEPS_IN_PATTERN;
            } else if (keys->Left) {
                *stepCount = (*stepCount + MAX_STEPS_IN_PATTERN - 1) % MAX_STEPS_IN_PATTERN;
            } else if (keys->Up) {
                for (uint8_t i = 0; i < 7; i++) {
                    if (steps[i] > *stepCount) {
                        *stepCount = steps[i];
                        break;
                    }
                }
            } else if (keys->Down) {
                for (uint8_t i = 7; i > 0; i--) {
                    if (steps[i - 1] < *stepCount) {
                        *stepCount = steps[i - 1];
                        break;
                    }
                }
            }
        } else if (col == 2) {
            patterns->save(*currentPatternId);
        } else if (col == 3) {
            patterns->load(*currentPatternId);
        }
        return VIEW_CHANGED;
    }
};

class App_View_PatternStepHeader : public App_View_TableField {
protected:
    const char* headers[5] = { "STP ", "I ", "NOT ", "VEL ", "SLID" };

public:
    void render(App_Renderer* renderer, uint8_t row, uint8_t col, uint8_t selectedRow, uint8_t selectedCol)
    {
        strcat(renderer->text, headers[col]);
    }
};

class App_View_PatternStep : public App_View_TableField {
protected:
    App_Patterns* patterns;
    uint8_t* currentPatternId;

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

    void render(App_Renderer* renderer, uint8_t row, uint8_t col, uint8_t selectedRow, uint8_t selectedCol)
    {
        uint8_t cursorLen[VIEW_PATTERN_COL] = { 0, 1, 3, 3, 3 };
        if (selectedRow == row && selectedCol == col) {
            renderer->setCursor(cursorLen[col]);
        }
        uint8_t stepPos = row - VIEW_PATTERN_ROW_HEADERS;
        Zic_Seq_Step* step = &patterns->patterns[*currentPatternId].steps[0][stepPos];
        switch (col) {
        case 0:
            sprintf(renderer->text + strlen(renderer->text), " %02d ", stepPos + 1);
            break;

        case 1:
            sprintf(renderer->text + strlen(renderer->text), "%c ", step->instrument + 'A');
            break;

        case 2:
            if (step->note == 0) {
                strcat(renderer->text, "--- ");
            } else {
                sprintf(renderer->text + strlen(renderer->text), "%2s%d ", Zic::getNoteStr(step->note), Zic::getNoteOctave(step->note));
            }
            break;

        case 3:
            sprintf(renderer->text + strlen(renderer->text), "%3d ", step->velocity);
            break;

        case 4:
            sprintf(renderer->text + strlen(renderer->text), "%s", step->slide ? "ON " : "OFF");
            break;
        }
    }

    uint8_t update(UiKeys* keys, App_Renderer* renderer, uint8_t row, uint8_t col)
    {
        Zic_Seq_Step* step = &patterns->patterns[*currentPatternId].steps[0][row - VIEW_PATTERN_ROW_HEADERS];
        if (col == 4) {
            step->slide = !step->slide;
        } else {
            int8_t directions[] = { 0, 1, 12, 10 };
            int8_t direction = 0;
            if (keys->Right) {
                direction = 1;
            } else if (keys->Left) {
                direction = -1;
            } else if (keys->Up) {
                direction = directions[col];
            } else if (keys->Down) {
                direction = -directions[col];
            }
            switch (col) {
            case 1:
                step->instrument = (step->instrument + INSTRUMENT_COUNT + direction) % INSTRUMENT_COUNT;
                break;
            case 2: {
                uint8_t note = range(step->note + direction, Zic::_NOTE_START - 1, Zic::_NOTE_END);
                step->note = range(note, Zic::_NOTE_START, Zic::_NOTE_END) != note ? 0 : note;
                break;
            }
            case 3:
                step->velocity = range(step->velocity + direction, 0, 127);
                break;
            }
        }
        return VIEW_CHANGED;
    }
};

class App_View_Pattern : public App_View_Table {
protected:
    App_Patterns* patterns;
    uint8_t currentPatternId = 0;

    App_View_PatternHeader headerField;
    App_View_PatternStepHeader stepHeaderField;
    App_View_PatternStep stepField;

    App_View_TableField* fields[VIEW_PATTERN_ROW * VIEW_PATTERN_COL] = {
        // clang-format off
        &headerField, &headerField, NULL, NULL, NULL,
        &headerField, &headerField, &headerField, &headerField, NULL,
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
        , patterns(_patterns)
        , headerField(_patterns, &currentPatternId)
        , stepField(_patterns, &currentPatternId)
    {
        initSelection();
    }

    void initDisplay(App_Renderer* renderer)
    {
        renderer->useColoredLabel(1, 4);
        renderer->useColoredHeader(0, 3);
        App_View_Table::initDisplay(renderer);
    }

    uint8_t update(UiKeys* keys, App_Renderer* renderer) override
    {
        uint8_t ret = App_View_Table::update(keys, renderer);
        setLastRow(patterns->patterns[currentPatternId].stepCount + VIEW_PATTERN_ROW_HEADERS);
        return ret;
    }

    void snapshot(App_Renderer* renderer) override
    {
        uint8_t id = currentPatternId;
        for (currentPatternId = 0; currentPatternId < PATTERN_COUNT; currentPatternId++) {
            renderer->startRow = 0;
            setLastRow(patterns->patterns[currentPatternId].stepCount + VIEW_PATTERN_ROW_HEADERS);
            for (uint8_t row = 0; row < lastRow; row += TABLE_VISIBLE_ROWS, renderer->startRow += TABLE_VISIBLE_ROWS) {
                render(renderer);
                saveFileContent(row == 0 ? "w" : "a", renderer->text, strlen(renderer->text),
                    "projects/current/patterns/pattern%03d_%02X.zic", currentPatternId + 1, currentPatternId + 1);
            }
        }
        currentPatternId = id;
    }
};

#endif