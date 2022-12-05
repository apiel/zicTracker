#ifndef APP_VIEW_GRID_H_
#define APP_VIEW_GRID_H_

#include "./app_tracks.h"
#include <app_core_renderer.h>
#include <app_core_util.h>
#include <app_core_view_table.h>

#define VIEW_GRID_ROW PATTERN_COMPONENT_COUNT
#define VIEW_GRID_COL TRACK_COUNT * 3

class App_View_GridField : public App_View_TableField {
protected:
    App_Tracks* tracks;
    bool editing = false;
    char* description;

public:
    App_View_GridField(App_Tracks* _tracks, char* _description)
        : tracks(_tracks)
        , description(_description)
    {
    }

    virtual bool isSelectable(uint8_t row, uint8_t col) override
    {
        return true;
    }

    void selected(App_Renderer* renderer, uint8_t row, uint8_t col) override
    {
        App_Audio_Track* track = tracks->tracks[uint8_t(col / 3) % TRACK_COUNT];
        Zic_Seq_PatternComponent* component = &track->components[(row) % PATTERN_COMPONENT_COUNT];

        renderer->setCursor(2, col % 3 == 0 ? 1 : 0);
        if (editing) {
            component = &newComponent;
        }

        if (col % 3 == 0) {
            if (component->pattern == NULL) {
                sprintf(description, "Pattern: to be selected");
            } else {
                sprintf(description, "Pattern: %02X (%d steps)", component->pattern->id + 1, component->pattern->stepCount);
            }
        } else if (col % 3 == 1) {
            sprintf(description, "Detune: %d semi tone", component->detune);
        } else if (col % 3 == 2) {
            sprintf(description, "Condition: %s %s",
                SEQ_CONDITIONS_NAMES[component->condition],
                SEQ_CONDITIONS_FULLNAMES[component->condition]);
        }
    }

    void render(App_Renderer* renderer, uint8_t row, uint8_t col, uint8_t selectedRow, uint8_t selectedCol)
    {
        App_Audio_Track* track = tracks->tracks[uint8_t(col / 3) % TRACK_COUNT];
        Zic_Seq_PatternComponent* component = &track->components[(row) % PATTERN_COMPONENT_COUNT];

        if (col % 3 == 0) {
            renderer->useColor(row + 1, col / 3 * 7, track->looper.isComponentPlaying(row) ? COLOR_PLAY : COLOR_MARKER);
            strcat(renderer->text,
                track->looper.isComponentPlaying(row) ? ">"
                                                      : (track->looper.isCurrentComponent(row) ? "*" : " "));
            if (component->pattern == NULL) {
                strcat(renderer->text, "--");
            } else {
                sprintf(renderer->text + strlen(renderer->text), "%02X", component->pattern->id + 1);
            }
        } else if (col % 3 == 1) {
            renderer->useColor(row + 1, col / 3 * 7 + 3, COLOR_LIGHT, 4);
            if (component->detune < 0) {
                sprintf(renderer->text + strlen(renderer->text), "-%c", alphanum[-component->detune]);
            } else {
                sprintf(renderer->text + strlen(renderer->text), "+%c", alphanum[component->detune]);
            }
        } else {
            strcat(renderer->text, SEQ_CONDITIONS_NAMES[component->condition]);
        }
    }

    Zic_Seq_PatternComponent* getComponent(uint8_t row, uint8_t col)
    {
        App_Audio_Track* track = tracks->tracks[uint8_t(col / 3) % TRACK_COUNT];
        return &track->components[(row) % PATTERN_COMPONENT_COUNT];
    }

    void updateStart(uint8_t row, uint8_t col)
    {
        newComponent.set(getComponent(row, col));
        editing = true;
    }

    void updateEnd(uint8_t row, uint8_t col)
    {
        getComponent(row, col)->set(&newComponent);
        editing = false;
    }

    uint8_t update(UiKeys* keys, App_Renderer* renderer, uint8_t row, uint8_t col)
    {
        int8_t directions[] = { 16, 12, 1 };
        int8_t direction = 0;
        if (keys->Right) {
            direction = 1;
        } else if (keys->Left) {
            direction = -1;
        } else if (keys->Up) {
            direction = directions[col % 3];
        } else if (keys->Down) {
            direction = -directions[col % 3];
        }

        switch (col % 3) {
        case 0: {
            int16_t id = newComponent.pattern == NULL ? -1 : newComponent.pattern->id;
            id = (id + direction) % PATTERN_COUNT;
            newComponent.pattern = id < 0 ? NULL : &patterns[id];
            break;
        }
        case 1: {
            newComponent.setDetune(newComponent.detune + direction);
            break;
        }
        case 2:
            newComponent.setCondition(newComponent.condition + direction);
            break;
        }
        return VIEW_CHANGED;
    }
};

class App_View_Grid : public App_View_Table {
protected:
    App_Tracks* tracks;
    char description[30] = "";

    App_View_TableField* field;

    App_View_TableField* fields[VIEW_GRID_ROW * VIEW_GRID_COL] = {
        // clang-format off
        field, field, field, field, field, field, field, field, field, field, field, field,
        field, field, field, field, field, field, field, field, field, field, field, field,
        field, field, field, field, field, field, field, field, field, field, field, field,
        field, field, field, field, field, field, field, field, field, field, field, field,
        field, field, field, field, field, field, field, field, field, field, field, field,
        field, field, field, field, field, field, field, field, field, field, field, field,
        field, field, field, field, field, field, field, field, field, field, field, field,
        field, field, field, field, field, field, field, field, field, field, field, field,
        // clang-format on
    };

public:
    App_View_Grid(App_Tracks* _tracks, Zic_Seq_Pattern* _patterns, App_View_TableField* field)
        : App_View_Table(fields, VIEW_GRID_ROW, VIEW_GRID_COL)
        , tracks(_tracks)
    {
        initSelection();
    }

    bool renderOn(uint8_t event) override
    {
        return true;
    }

    void initDisplay(App_Renderer* renderer)
    {
    }

    void render(App_Renderer* renderer)
    {
        renderer->useColoredRow();
        renderer->useColoredRow(9, COLOR_MEDIUM);
        strcpy(renderer->text, "");
        for (uint8_t i = 0; i < TRACK_COUNT; i++) {
            sprintf(renderer->text + strlen(renderer->text), " TRACK%d", i + 1);
        }
        strcat(renderer->text, "\n");
        App_View_Table::render(renderer);
        sprintf(renderer->text + strlen(renderer->text), " %s", description);
    }
};

#endif