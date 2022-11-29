#ifndef APP_VIEW_INSTRUMENT_H_
#define APP_VIEW_INSTRUMENT_H_

#include "./app_tracks.h"
#include <PdBase.hpp>
#include <PdObject.h>
#include <app_core_renderer.h>
#include <app_core_view_table.h>

#define VIEW_INSTR_ROW 8
#define VIEW_INSTR_COL 3
#define VIEW_INSTR_LABELS 7

class App_View_InstrumentCcRow : public App_View_TableField {
protected:
    App_Tracks* tracks;
    uint8_t* instrument;

public:
    App_View_InstrumentCcRow(App_Tracks* _tracks, uint8_t* _instrument)
        : tracks(_tracks)
        , instrument(_instrument)
    {
    }

    void render(App_Renderer* renderer, uint8_t row, uint8_t col, uint8_t selectedRow, uint8_t selectedCol)
    {
        strcat(renderer->text, "hello");
    }

    uint8_t update(UiKeys* keys, App_Renderer* renderer, uint8_t row, uint8_t col)
    {
        return VIEW_CHANGED;
    }
};

class App_View_Instrument : public App_View_Table {
protected:
    App_Tracks* tracks;

    PdObject pdObject;
    pd::PdBase pd;
    pd::Patch patch;

    uint8_t instrument = 0;

    App_View_InstrumentCcRow ccField;
    App_View_TableField* fields[VIEW_INSTR_ROW * VIEW_INSTR_COL] = {
        // clang-format off
        NULL, NULL, NULL,
        &ccField, NULL, NULL,
        &ccField, NULL, NULL,
        &ccField, NULL, NULL,
        &ccField, NULL, NULL,
        &ccField, NULL, NULL,
        &ccField, NULL, NULL,
        &ccField, NULL, NULL,
        // clang-format on
    };

public:
    App_View_Instrument(App_Tracks* _tracks)
        : App_View_Table(fields, VIEW_INSTR_ROW, VIEW_INSTR_COL)
        , tracks(_tracks)
        , pdObject(255)
        , ccField(_tracks, &instrument)
    {
        initSelection();

        if (!pd.init(0, APP_CHANNELS, SAMPLE_RATE)) {
            APP_LOG("Could not init pd\n");
        }
        // pd.computeAudio(true);
        patch = pd.openPatch("main.pd", "puredata/synth01");
        pd.setReceiver(&pdObject);
        // pd.setMidiReceiver(&pdObject);
        pd.sendControlChange(1, 1, 10);
    }

    ~App_View_Instrument()
    {
        pd.closePatch(patch);
        pd.clear();
    }

    void initDisplay(App_Renderer* renderer)
    {
        renderer->useColor(0, 255, 0, 6);
        App_View_Table::initDisplay(renderer);
    }

    // uint8_t update(UiKeys* keys, App_Renderer* renderer) override
    // {
    //     // if (keys->Edit) {
    //     pd.sendControlChange(1, 1, 10);
    //     printf("update instr.\n");
    //     // }
    //     return App_View_Table::update(keys, renderer);
    // }
};

#endif