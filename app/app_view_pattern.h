#ifndef APP_VIEW_PATTERN_H_
#define APP_VIEW_PATTERN_H_

#include <app_core_renderer.h>
#include <app_core_util.h>
#include <app_core_view.h>
#include <app_core_view_table.h>

#define VIEW_PATTERN_ROW_HEADERS 4
#define VIEW_PATTERN_ROW (VIEW_PATTERN_ROW_HEADERS + MAX_STEPS_IN_PATTERN)
// #define VIEW_PATTERN_ROW (VIEW_PATTERN_ROW_HEADERS + 6)
#define VIEW_PATTERN_COL 4 * INSTRUMENT_COUNT

#define VELOCITY_ARR uint8_t velocity[6] = { 50, 70, 85, 100, 110, 127 };

class App_View_PatternHeader : public App_View_TableField {
protected:
    Zic_Seq_Pattern* patterns;
    uint8_t* currentPatternId;

    const char* headers[2] = { "PAT ", "LEN" };

    void renderHeader(App_Renderer* renderer, uint8_t col)
    {
        strcat(renderer->text, headers[col]);
    }

public:
    App_View_PatternHeader(Zic_Seq_Pattern* _patterns, uint8_t* _currentPatternId)
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
            uint8_t cursorLen[] = { 3, 3, 5, 5 };
            if (selectedRow == row && selectedCol == col) {
                renderer->setCursor(cursorLen[col]);
            }
            switch (col) {
            case 0:
                sprintf(renderer->text + strlen(renderer->text), " %02X ", *currentPatternId + 1);
                break;

            case 1:
                sprintf(renderer->text + strlen(renderer->text), "%3d ", patterns[*currentPatternId].stepCount);
                break;
            case 2:
                strcat(renderer->text, "RESET ");
                break;
            case 3:
                strcat(renderer->text, "CLEAR ");
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
            uint8_t* stepCount = &patterns[*currentPatternId].stepCount;
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
            // TODO restore pattern
        }
        return VIEW_CHANGED;
    }
};

class App_View_PatternStepHeader : public App_View_TableField {
protected:
    // const char* headers[5] = { "STP ", "I ", "NOT ", "VEL ", "SLID" };

public:
    void render(App_Renderer* renderer, uint8_t row, uint8_t col, uint8_t selectedRow, uint8_t selectedCol)
    {
        // strcat(renderer->text, headers[col]);
        sprintf(renderer->text + strlen(renderer->text), "Inst.%c ", col + 'A');
    }
};

class App_View_PatternStep : public App_View_TableField {
protected:
    Zic_Seq_Pattern* patterns;
    uint8_t* currentPatternId;

    VELOCITY_ARR

    uint8_t getVel(Zic_Seq_Step* step)
    {
        uint8_t vel = 0;
        for (; vel < 6; vel++) {
            if (step->velocity <= velocity[vel]) {
                return vel;
            }
        }
        return vel;
    }

public:
    App_View_PatternStep(Zic_Seq_Pattern* _patterns, uint8_t* _currentPatternId)
        : patterns(_patterns)
        , currentPatternId(_currentPatternId)
    {
    }

    bool isSelectable(uint8_t row, uint8_t col) override
    {
        return true;
    }

    void selected(App_Renderer* renderer, uint8_t row, uint8_t col) override
    {
        renderer->setCursor(col % 4 == 0 ? 3 : 1);
    }

    void render(App_Renderer* renderer, uint8_t row, uint8_t col, uint8_t selectedRow, uint8_t selectedCol)
    {
        uint8_t inst = col / 4;
        uint8_t stepPos = row - VIEW_PATTERN_ROW_HEADERS;
        Zic_Seq_Step* step = &patterns[*currentPatternId].steps[inst][stepPos];
        switch (col % 4) {
        case 0:
            if (step->note == 0) {
                strcat(renderer->text, "---");
            } else {
                sprintf(renderer->text + strlen(renderer->text), "%2s%d", Zic::getNoteUnderscore(step->note), Zic::getNoteOctave(step->note));
            }
            break;

        case 1:
            renderer->useColor(row, col / 4 * 7 + 3, COLOR_MEDIUM);
            strcat(renderer->text, charLevel(getVel(step) + 1));
            break;

        case 2:
            sprintf(renderer->text + strlen(renderer->text), "%d", step->condition);
            break;

        case 3:
            // FIXME len differ between "⤦" and " " "♪" " "
            // strcat(renderer->text, step->slide ? "⤸" : " ");
            strcat(renderer->text, step->slide ? "⤸ " : "  ");
            // strcat(renderer->text, step->slide ? "_" : " ");
            break;
        }
    }

    uint8_t update(UiKeys* keys, App_Renderer* renderer, uint8_t row, uint8_t col)
    {
        uint8_t inst = col / 4;
        Zic_Seq_Step* step = &patterns[*currentPatternId].steps[inst][row - VIEW_PATTERN_ROW_HEADERS];
        col %= 4;
        if (col == 3) {
            step->slide = !step->slide;
        } else {
            int8_t directions[] = { 12, 1, 1, 0 };
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
            case 0: {
                // FIXME from no note (0) to C0 need to fix right key
                // TODO when no note, reuse the last inserted note from previous step
                uint8_t note = range(step->note + direction, Zic::_NOTE_START - 1, Zic::_NOTE_END);
                step->note = range(note, Zic::_NOTE_START, Zic::_NOTE_END) != note ? 0 : note;
                break;
            }
            case 1: {
                uint8_t vel = range(getVel(step) + direction, 0, 5);
                step->velocity = velocity[vel];
                break;
            }
            case 2:
                step->condition = range(step->condition + direction, 1, 9);
                break;
            }
        }
        return VIEW_CHANGED;
    }
};

class App_View_Pattern : public App_View_Table {
protected:
    Zic_Seq_Pattern* patterns;
    uint8_t currentPatternId = 0;

    App_View_PatternHeader headerField;
    App_View_PatternStepHeader stepHeaderField;
    App_View_PatternStep stepField;

    App_View_TableField* fields[VIEW_PATTERN_ROW * VIEW_PATTERN_COL] = {
        // clang-format off
        &headerField, &headerField, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        &headerField, &headerField, &headerField, &headerField, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL,  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        &stepHeaderField, &stepHeaderField, &stepHeaderField, &stepHeaderField, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,

        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField, &stepField,
        // clang-format on
    };

    App_View_Pattern(Zic_Seq_Pattern* _patterns)
        : App_View_Table(fields, VIEW_PATTERN_ROW, VIEW_PATTERN_COL)
        , patterns(_patterns)
        , headerField(_patterns, &currentPatternId)
        , stepField(_patterns, &currentPatternId)
    {
        initSelection();
    }

public:
    static App_View_Pattern* instance;

    static App_View_Pattern* getInstance(Zic_Seq_Pattern* _patterns)
    {
        if (!instance) {
            instance = new App_View_Pattern(_patterns);
        }
        return instance;
    }

    void focusView() override
    {
        // setGridSelection();
        // App_Audio_Track * track = App_Audio_Track::getInstance();
    }

    void initDisplay(App_Renderer* renderer)
    {
        renderer->useColoredRow(0);
        renderer->useColoredRow(3);
        App_View_Table::initDisplay(renderer);
    }

    uint8_t update(UiKeys* keys, App_Renderer* renderer) override
    {
        uint8_t ret = App_View_Table::update(keys, renderer);
        setLastRow(patterns[currentPatternId].stepCount + VIEW_PATTERN_ROW_HEADERS);
        return ret;
    }

    // TODO override render to show description of selected field like on the grid view

    const char* snapshotPath = "projects/current/patterns/pattern%03d_%02X.zic";

    void snapshot(App_Renderer* renderer) override
    {
        uint8_t id = currentPatternId;
        for (currentPatternId = 0; currentPatternId < PATTERN_COUNT; currentPatternId++) {
            renderer->startRow = 0;
            setLastRow(patterns[currentPatternId].stepCount + VIEW_PATTERN_ROW_HEADERS);
            for (uint8_t row = 0; row < lastRow; row += TABLE_VISIBLE_ROWS, renderer->startRow += TABLE_VISIBLE_ROWS) {
                render(renderer);
                saveFileContent(row == 0 ? "w" : "a", renderer->text, strlen(renderer->text),
                    snapshotPath, currentPatternId + 1, currentPatternId + 1);
            }
        }
        currentPatternId = id;
    }

    void loadSnapshot() override
    {
        VELOCITY_ARR

        for (uint8_t id = 0; id < PATTERN_COUNT; id++) {
            char filename[MAX_FILENAME];
            snprintf(filename, MAX_FILENAME, snapshotPath, id + 1, id + 1);
            Zic_File file(filename, "r");
            if (file.isOpen()) {
                Zic_Seq_Pattern* pattern = &patterns[id];
                pattern->id = id;
                file.seekFromStart(13);
                char lenStr[3];
                file.read(lenStr, 2);
                lenStr[2] = 0;
                pattern->stepCount = atoi(lenStr);
                file.seekFromCurrent(44);
                char stepStr[11];
                for (uint8_t s = 0; s < pattern->stepCount; s++) {
                    for (uint8_t i = 0; i < INSTRUMENT_COUNT; i++) {
                        if (!file.read(stepStr, 11)) {
                            break;
                        }
                        stepStr[10] = 0;
                        Zic_Seq_Step* step = &pattern->steps[i][s];
                        step->condition = stepStr[6] - '0';
                        step->slide = stepStr[8] == -92;
                        step->velocity = velocity[getLevel(*(uint16_t*)(stepStr + 4))];
                        step->note = stepStr[0] == '-' ? 0 : Zic::charNotetoInt(stepStr[0], stepStr[1], stepStr[2]);
                    }
                    file.seekFromCurrent(1);
                }
                file.close();
            }
        }
    }
};

App_View_Pattern* App_View_Pattern::instance = NULL;

#endif