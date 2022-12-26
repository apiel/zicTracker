#ifndef APP_VIEW_GRID_INSTRUMENT_H_
#define APP_VIEW_GRID_INSTRUMENT_H_

#include "./app_tracks.h"
#include <app_core_renderer.h>
#include <app_core_util.h>
#include <app_core_view_table.h>

#include "./app_view_grid.h"

class App_View_GridInstrumentField : public App_View_GridField {
protected:
    App_State_Track* getState(uint8_t row, uint8_t col)
    {
        return &getTrack(col)->state[(row) % APP_TRACK_STATE_SIZE];
    }

public:
    App_View_GridInstrumentField(char* _description)
        : App_View_GridField(_description)
    {
    }

    virtual bool isSelectable(uint8_t row, uint8_t col) override
    {
        return col % 3 != 2;
    }

    void selectCol0(App_Renderer* renderer, uint8_t row, uint8_t col)
    {
        App_State_Track* state = getState(row, col);
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
        char* name = getState(row, col)->patchFilename;
        sprintf(renderer->text + strlen(renderer->text), "%c%c", name[0], name[1]);
    }

    void renderCol1(App_Renderer* renderer, uint8_t row, uint8_t col, bool isSelected)
    {
        if (getState(row, col)->isPatchEmpty()) {
            strcat(renderer->text, " -- ");
        } else {
            sprintf(renderer->text + strlen(renderer->text), " %02X ", getState(row, col)->preset);
        }
    }

    void updatePatch(uint8_t row, uint8_t col, int8_t direction)
    {
        App_Audio_Track* track = getTrack(col);
        getState(row, col)->setNextPatch(direction, track->getPatchDirectory());
        // isCurrentState(uint8_t pos)
        if (track->isCurrentState(row)) {
            track->loadPatch();
        }
    }

    uint8_t updateCol0(UiKeys* keys, App_Renderer* renderer, uint8_t row, uint8_t col)
    {
        if (keys->Right || keys->Down) {
            updatePatch(row, col, +1);
            return VIEW_CHANGED;
        } else if (keys->Left || keys->Up) {
            updatePatch(row, col, -1);
            return VIEW_CHANGED;
        }
        return VIEW_NONE;
    }

    uint8_t updateCol1(UiKeys* keys, App_Renderer* renderer, uint8_t row, uint8_t col)
    {
        int8_t direction = 0;
        if (keys->Right) {
            direction = +1;
        } else if (keys->Left) {
            direction = -1;
        } else if (keys->Down) {
            direction = -16;
        } else if (keys->Up) {
            direction = +16;
        }
        getState(row, col)->preset += direction;
        App_Audio_Track* track = getTrack(col);
        if (track->isCurrentState(row)) {
            track->loadPatch();
        }
        return VIEW_CHANGED;
    }
};

class App_View_GridInstrument : public App_View_Grid {
protected:
    App_View_GridInstrumentField field;

    App_View_GridInstrument()
        : App_View_Grid(&field)
        , field(description)
    {
        initSelection();
    }

public:
    static App_View_GridInstrument* instance;

    static App_View_GridInstrument* getInstance()
    {
        if (!instance) {
            instance = new App_View_GridInstrument();
        }
        return instance;
    }

    void setGridSelection() override
    {
        selectedCol = gridSelectedCol % 3 == 2 ? gridSelectedCol - 1 : gridSelectedCol;
        selectedRow = gridSelectedRow;
    }
};

App_View_GridInstrument* App_View_GridInstrument::instance = NULL;

#endif