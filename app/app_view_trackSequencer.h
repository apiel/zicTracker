#ifndef APP_VIEW_TRACK_SEQUENCER_H_
#define APP_VIEW_TRACK_SEQUENCER_H_

#include "./app_renderer.h"
#include "./app_tracks.h"
#include "./app_util.h"
#include "./app_view_table.h"

#define VIEW_TRACK_SEQUENCER_ROW (1 + PATTERN_COMPONENT_COUNT)
#define VIEW_TRACK_SEQUENCER_COL TRACK_COUNT * 3

class App_View_TrackSequencerHeader : public App_View_TableField {
public:
    void render(App_Renderer* renderer, uint8_t row, uint8_t col, uint8_t selectedRow, uint8_t selectedCol)
    {
        sprintf(renderer->text + strlen(renderer->text), " TRACK%d", col + 1);
    }
};

class App_View_TrackSequencerPat : public App_View_TableField {
protected:
    App_Tracks* tracks;
    Zic_Seq_PatternComponent newComponent;
    bool updating = false;

public:
    App_View_TrackSequencerPat(App_Tracks* _tracks)
        : tracks(_tracks)
    {
    }

    bool isSelectable(uint8_t row, uint8_t col) override
    {
        return true;
    }

    void render(App_Renderer* renderer, uint8_t row, uint8_t col, uint8_t selectedRow, uint8_t selectedCol)
    {
        App_Audio_Track* track = tracks->tracks[uint8_t(col / 3) % TRACK_COUNT];
        Zic_Seq_PatternComponent* component = &track->components[(row - 1) % PATTERN_COMPONENT_COUNT];

        if (selectedRow == row && selectedCol == col) {
            renderer->setCursor(2, col % 3 == 0 ? 1 : 0);
            if (updating) {
                component = &newComponent;
            }
        }
        if (col % 3 == 0) {
            strcat(renderer->text,
                track->looper.isComponentPlaying(row - 1) ? ">"
                                                          : (track->looper.isCurrentComponent(row - 1) ? "*" : " "));
            if (component->pattern == NULL) {
                strcat(renderer->text, "--");
            } else {
                sprintf(renderer->text + strlen(renderer->text), "%02X", component->pattern->id + 1);
            }
        } else if (col % 3 == 1) {
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
        return &track->components[(row - 1) % PATTERN_COMPONENT_COUNT];
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
            newComponent.pattern = id < 0 ? NULL : &tracks->patterns->patterns[id];
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

class App_View_TrackSequencer : public App_View_Table {
protected:
    App_View_TrackSequencerHeader header;
    App_View_TrackSequencerPat patField;
    App_Tracks* tracks;

    App_View_TableField* fields[VIEW_TRACK_SEQUENCER_ROW * VIEW_TRACK_SEQUENCER_COL] = {
        // clang-format off
        &header, &header, &header, &header, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        &patField, &patField, &patField, &patField, &patField, &patField, &patField, &patField, &patField, &patField, &patField, &patField,
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

public:
    App_View_TrackSequencer(App_Tracks* _tracks)
        : App_View_Table(fields, VIEW_TRACK_SEQUENCER_ROW, VIEW_TRACK_SEQUENCER_COL)
        , patField(_tracks)
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
        renderer->useColoredHeader();
        App_View_Table::initDisplay(renderer);
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

                printf("trk%d(%d) pat: %s (%d), detune: %s, condition: %s\n",
                    (i % TRACK_COUNT) + 1, i / TRACK_COUNT,
                    pat, pat[0] == '-' ? -1 : (int)strtol(pat, NULL, 16) - 1,
                    detune, condition);

                App_Audio_Track* track = tracks->tracks[i % TRACK_COUNT];
                Zic_Seq_PatternComponent* component = &track->components[i / TRACK_COUNT];
                component->pattern = pat[0] == '-' ? NULL : &tracks->patterns->patterns[strtol(pat, NULL, 16) - 1];
                component->setDetune((detune[0] == '-' ? -1 : 1) * alphanumToInt(detune[1]));
                component->setCondition(condition);
            }

            file.close();
        }
    }
};

#endif