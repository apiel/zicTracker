#ifndef APP_VIEW_GRID_INSTRUMENT_H_
#define APP_VIEW_GRID_INSTRUMENT_H_

#include "./app_tracks.h"
#include <app_core_renderer.h>
#include <app_core_util.h>
#include <app_core_view_table.h>

#include "./app_view_grid.h"

class App_View_GridInstrumentField : public App_View_GridField {
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
        sprintf(description, "Patch: to be selected");
    }

    void selectCol1(App_Renderer* renderer, uint8_t row, uint8_t col)
    {
        renderer->setCursor(2, 1);
        sprintf(description, "Preset: to be selected");
    }

    void renderCol0(App_Renderer* renderer, uint8_t row, uint8_t col, bool isSelected)
    {
        App_Audio_Track* track = tracks->tracks[uint8_t(col / 3) % TRACK_COUNT];
        strcat(renderer->text,
            track->looper.isComponentPlaying(row) ? ">"
                                                  : (track->looper.isCurrentComponent(row) ? "*" : " "));
        strcat(renderer->text, "--");
    }

    void renderCol1(App_Renderer* renderer, uint8_t row, uint8_t col, bool isSelected)
    {
        strcat(renderer->text, " -- ");
    }

    uint8_t updateCol0(UiKeys* keys, App_Renderer* renderer, uint8_t row, uint8_t col)
    {
        return VIEW_CHANGED;
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