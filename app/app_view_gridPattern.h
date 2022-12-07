#ifndef APP_VIEW_GRID_PATTERN_H_
#define APP_VIEW_GRID_PATTERN_H_

#include "./app_tracks.h"
#include <app_core_renderer.h>
#include <app_core_util.h>
#include <app_core_view_table.h>

#include "./app_view_grid.h"

class App_View_GridPatternField : public App_View_GridField {
protected:
    Zic_Seq_Pattern* patterns;
    Zic_Seq_PatternComponent newComponent;
    bool editing = false;

public:
    App_View_GridPatternField(Zic_Seq_Pattern* _patterns, char* _description)
        : App_View_GridField(_description)
        , patterns(_patterns)
    {
    }

    Zic_Seq_PatternComponent* getComponent(uint8_t row, uint8_t col, bool isSelected = true)
    {
        if (isSelected && editing) {
            return &newComponent;
        }
        return &getTrack(col)->components[(row) % APP_TRACK_STATE_SIZE];
    }

    void selectCol0(App_Renderer* renderer, uint8_t row, uint8_t col)
    {
        Zic_Seq_PatternComponent* component = getComponent(row, col);
        if (component->pattern == NULL) {
            sprintf(description, "Pattern: to be selected");
        } else {
            sprintf(description, "Pattern: %02X (%d steps)", component->pattern->id + 1, component->pattern->stepCount);
        }
    }

    void selectCol1(App_Renderer* renderer, uint8_t row, uint8_t col)
    {
        Zic_Seq_PatternComponent* component = getComponent(row, col);
        sprintf(description, "Detune: %d semi tone", component->detune);
    }

    void selectCol2(App_Renderer* renderer, uint8_t row, uint8_t col)
    {
        Zic_Seq_PatternComponent* component = getComponent(row, col);
        sprintf(description, "Condition: %s %s",
            SEQ_CONDITIONS_NAMES[component->condition],
            SEQ_CONDITIONS_FULLNAMES[component->condition]);
    }

    void renderCol0(App_Renderer* renderer, uint8_t row, uint8_t col, bool isSelected)
    {
        Zic_Seq_PatternComponent* component = getComponent(row, col, isSelected);
        if (component->pattern == NULL) {
            strcat(renderer->text, "--");
        } else {
            sprintf(renderer->text + strlen(renderer->text), "%02X", component->pattern->id + 1);
        }
    }

    void renderCol1(App_Renderer* renderer, uint8_t row, uint8_t col, bool isSelected)
    {
        Zic_Seq_PatternComponent* component = getComponent(row, col, isSelected);
        renderer->useColor(row + 1, col / 3 * 7 + 3, COLOR_LIGHT, 4);
        if (component->detune < 0) {
            sprintf(renderer->text + strlen(renderer->text), "-%c", alphanum[-component->detune]);
        } else {
            sprintf(renderer->text + strlen(renderer->text), "+%c", alphanum[component->detune]);
        }
    }

    void renderCol2(App_Renderer* renderer, uint8_t row, uint8_t col, bool isSelected)
    {
        Zic_Seq_PatternComponent* component = getComponent(row, col, isSelected);
        strcat(renderer->text, SEQ_CONDITIONS_NAMES[component->condition]);
    }

    void updateStart(uint8_t row, uint8_t col)
    {
        newComponent.set(getComponent(row, col));
        editing = true;
    }

    void updateEnd(uint8_t row, uint8_t col)
    {
        editing = false;
        getComponent(row, col)->set(&newComponent);
    }

    uint8_t updateCol0(UiKeys* keys, App_Renderer* renderer, uint8_t row, uint8_t col)
    {
        int8_t direction = getDirection(keys, 16);
        int16_t id = newComponent.pattern == NULL ? -1 : newComponent.pattern->id;
        id = (id + direction) % PATTERN_COUNT;
        newComponent.pattern = id < 0 ? NULL : &patterns[id];
        return VIEW_CHANGED;
    }
    uint8_t updateCol1(UiKeys* keys, App_Renderer* renderer, uint8_t row, uint8_t col)
    {
        int8_t direction = getDirection(keys, 12);
        newComponent.setDetune(newComponent.detune + direction);
        return VIEW_CHANGED;
    }
    uint8_t updateCol2(UiKeys* keys, App_Renderer* renderer, uint8_t row, uint8_t col)
    {
        int8_t direction = getDirection(keys, 1);
        newComponent.setCondition(newComponent.condition + direction);
        return VIEW_CHANGED;
    }

    uint8_t getDirection(UiKeys* keys, uint8_t bigStep)
    {
        if (keys->Right) {
            return 1;
        } else if (keys->Left) {
            return -1;
        } else if (keys->Up) {
            return bigStep;
        } else if (keys->Down) {
            return -bigStep;
        }
        return 0;
    }
};

class App_View_GridPattern : public App_View_Grid {
protected:
    Zic_Seq_Pattern* patterns;
    App_View_GridPatternField field;

    App_View_GridPattern(Zic_Seq_Pattern* _patterns)
        : App_View_Grid(&field)
        , patterns(_patterns)
        , field(_patterns, description)
    {
        initSelection();
    }

public:
    static App_View_GridPattern* instance;

    static App_View_GridPattern* getInstance(Zic_Seq_Pattern* _patterns)
    {
        if (!instance) {
            instance = new App_View_GridPattern(_patterns);
        }
        return instance;
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

            for (uint8_t i = 0; i < APP_TRACK_STATE_SIZE * TRACK_COUNT; i++) {
                file.seekFromCurrent(i % TRACK_COUNT == 0 ? 2 : 1);
                char pat[3];
                file.read(pat, 2);
                pat[2] = '\0';
                char detune[2];
                file.read(detune, 2);
                char condition[2];
                file.read(condition, 2);

                App_Audio_Track* track = App_Tracks::getInstance()->tracks[i % TRACK_COUNT];
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