#ifndef APP_VIEW_GRID_PATTERN_H_
#define APP_VIEW_GRID_PATTERN_H_

#include "./app_tracks.h"
#include <app_core_renderer.h>
#include <app_core_util.h>
#include <app_core_view_table.h>

#define VIEW_GRID_PATTERN_ROW PATTERN_COMPONENT_COUNT
#define VIEW_GRID_PATTERN_COL TRACK_COUNT * 3

class App_View_GridPatternField : public App_View_TableField {
protected:
    App_Tracks* tracks;
    Zic_Seq_Pattern* patterns;
    Zic_Seq_PatternComponent newComponent;
    bool updating = false;
    char* description;

public:
    App_View_GridPatternField(App_Tracks* _tracks, Zic_Seq_Pattern* _patterns, char* _description)
        : tracks(_tracks)
        , patterns(_patterns)
        , description(_description)
    {
    }

    bool isSelectable(uint8_t row, uint8_t col) override
    {
        return true;
    }

    void render(App_Renderer* renderer, uint8_t row, uint8_t col, uint8_t selectedRow, uint8_t selectedCol)
    {
        App_Audio_Track* track = tracks->tracks[uint8_t(col / 3) % TRACK_COUNT];
        Zic_Seq_PatternComponent* component = &track->components[(row) % PATTERN_COMPONENT_COUNT];

        if (selectedRow == row && selectedCol == col) {
            renderer->setCursor(2, col % 3 == 0 ? 1 : 0);
            if (updating) {
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
        updating = true;
    }

    void updateEnd(uint8_t row, uint8_t col)
    {
        getComponent(row, col)->set(&newComponent);
        updating = false;
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

class App_View_GridPattern : public App_View_Table {
protected:
    Zic_Seq_Pattern* patterns;
    App_View_GridPatternField patField;
    App_Tracks* tracks;
    char description[30] = "";

    App_View_TableField* fields[VIEW_GRID_PATTERN_ROW * VIEW_GRID_PATTERN_COL] = {
        // clang-format off
        &patField, &patField, &patField, &patField, &patField, &patField, &patField, &patField, &patField, &patField, &patField, &patField,
        &patField, &patField, &patField, &patField, &patField, &patField, &patField, &patField, &patField, &patField, &patField, &patField,
        &patField, &patField, &patField, &patField, &patField, &patField, &patField, &patField, &patField, &patField, &patField, &patField,
        &patField, &patField, &patField, &patField, &patField, &patField, &patField, &patField, &patField, &patField, &patField, &patField,
        &patField, &patField, &patField, &patField, &patField, &patField, &patField, &patField, &patField, &patField, &patField, &patField,
        &patField, &patField, &patField, &patField, &patField, &patField, &patField, &patField, &patField, &patField, &patField, &patField,
        &patField, &patField, &patField, &patField, &patField, &patField, &patField, &patField, &patField, &patField, &patField, &patField,
        &patField, &patField, &patField, &patField, &patField, &patField, &patField, &patField, &patField, &patField, &patField, &patField,
        // clang-format on
    };

    App_View_GridPattern(App_Tracks* _tracks, Zic_Seq_Pattern* _patterns)
        : App_View_Table(fields, VIEW_GRID_PATTERN_ROW, VIEW_GRID_PATTERN_COL)
        , patterns(_patterns)
        , patField(_tracks, _patterns, description)
        , tracks(_tracks)
    {
        initSelection();
    }

public:
    static App_View_GridPattern* instance;

    static App_View_GridPattern* getInstance(App_Tracks* _tracks, Zic_Seq_Pattern* _patterns)
    {
        if (!instance) {
            instance = new App_View_GridPattern(_tracks, _patterns);
        }
        return instance;
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

    const char* snapshotPath = "projects/current/sequencer.zic";

    void snapshot(App_Renderer* renderer) override
    {
        render(renderer);
        saveFileContent(renderer->text, strlen(renderer->text), snapshotPath);
    }

    void loadSnapshot() override
    {
        Zic_File file(snapshotPath, "r");
        if (file.isOpen()) {
            file.seekFromStart(28);

            for (uint8_t i = 0; i < PATTERN_COMPONENT_COUNT * TRACK_COUNT; i++) {
                file.seekFromCurrent(i % TRACK_COUNT == 0 ? 2 : 1);
                char pat[3];
                file.read(pat, 2);
                pat[2] = '\0';
                char detune[2];
                file.read(detune, 2);
                char condition[2];
                file.read(condition, 2);

                App_Audio_Track* track = tracks->tracks[i % TRACK_COUNT];
                Zic_Seq_PatternComponent* component = &track->components[i / TRACK_COUNT];
                component->pattern = pat[0] == '-' ? NULL : &patterns[strtol(pat, NULL, 16) - 1];
                component->setDetune((detune[0] == '-' ? -1 : 1) * alphanumToInt(detune[1]));
                component->setCondition(condition);
            }

            file.close();
        }
    }
};

App_View_GridPattern* App_View_GridPattern::instance = NULL;

#endif