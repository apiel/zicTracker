#ifndef APP_VIEW_GRID_INSTRUMENT_H_
#define APP_VIEW_GRID_INSTRUMENT_H_

#include "./app_tracks.h"
#include <app_core_renderer.h>
#include <app_core_util.h>
#include <app_core_view_table.h>

#include "./app_view_grid.h"

class App_View_GridInstrumentField : public App_View_GridField {
protected:
    App_Audio_Track* getTrack(uint8_t col)
    {
        return tracks->tracks[uint8_t(col / 3) % TRACK_COUNT];
    }

    App_Audio_TrackState* getState(uint8_t row, uint8_t col)
    {
        return &getTrack(col)->state[(row) % APP_TRACK_STATE_SIZE];
    }

public:
    App_View_GridInstrumentField(App_Tracks* _tracks, char* _description)
        : App_View_GridField(_tracks, _description)
    {
    }

    virtual bool isSelectable(uint8_t row, uint8_t col) override
    {
        return col % 3 != 2;
    }

    void selectCol0(App_Renderer* renderer, uint8_t row, uint8_t col)
    {
        App_Audio_TrackState* state = getState(row, col);
        if (state->isPatchEmpty()) {
            sprintf(description, "Patch: to be selected");
        } else {
            sprintf(description, "Patch: %s", state->patchFilename);
        }
    }

    void selectCol1(App_Renderer* renderer, uint8_t row, uint8_t col)
    {
        renderer->setCursor(2, 1);
        sprintf(description, "Preset: to be selected");
    }

    void renderCol0(App_Renderer* renderer, uint8_t row, uint8_t col, bool isSelected)
    {
        // char name[40];
        // snprintf(name, 40, "%s", getState(row, col)->patchFilename);
        // name[2] = '\0';
        // // strcat(renderer->text, name);
        // sprintf(renderer->text, "%s", name);

        char* name = getState(row, col)->patchFilename;
        // char name2[3];
        // name2[0] = name[0];
        // name2[1] = name[1];
        // name2[2] = '\0';

        sprintf(renderer->text + strlen(renderer->text), "%c%c", name[0], name[1]);

        // if (name[1] != '-')
        //     printf(">>>>>>>>>>>>>>>>>>>>>> %c%c\n", name[0], name[1]);

        // char name2[2]={name[0], name[1]};

        // sprintf(renderer->text + strlen(renderer->text), "00");
        // strcat(renderer->text, name2);
        // sprintf(renderer->text, "%s", name2);
    }

    void renderCol1(App_Renderer* renderer, uint8_t row, uint8_t col, bool isSelected)
    {
        strcat(renderer->text, " -- ");
    }

    uint8_t updateCol0(UiKeys* keys, App_Renderer* renderer, uint8_t row, uint8_t col)
    {
        if (keys->Right || keys->Down) {
            getState(row, col)->setNextPatch(+1);
            return VIEW_CHANGED;
        } else if (keys->Left || keys->Up) {
            getState(row, col)->setNextPatch(-1);
            return VIEW_CHANGED;
        }
        return VIEW_NONE;
    }

    uint8_t updateCol1(UiKeys* keys, App_Renderer* renderer, uint8_t row, uint8_t col)
    {
        return VIEW_CHANGED;
    }
};

class App_View_GridInstrument : public App_View_Grid {
protected:
    App_View_GridInstrumentField field;
    App_Tracks* tracks;

    App_View_GridInstrument(App_Tracks* _tracks)
        : App_View_Grid(&field)
        , field(_tracks, description)
        , tracks(_tracks)
    {
        initSelection();
    }

public:
    static App_View_GridInstrument* instance;

    static App_View_GridInstrument* getInstance(App_Tracks* _tracks)
    {
        if (!instance) {
            instance = new App_View_GridInstrument(_tracks);
        }
        return instance;
    }
};

App_View_GridInstrument* App_View_GridInstrument::instance = NULL;

#endif