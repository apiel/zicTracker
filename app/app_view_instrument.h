#ifndef APP_VIEW_INSTRUMENT_H_
#define APP_VIEW_INSTRUMENT_H_

#include "./app_tracks.h"
#include <app_core_renderer.h>
#include <app_core_view_js.h>

#include <duktape.h>

#define VIEW_INSTR_ROW 8
#define VIEW_INSTR_COL 3
#define VIEW_INSTR_LABELS 7

class App_View_Instrument : public App_View_JS {
protected:
    App_Tracks* tracks;

    App_View_Instrument(App_Tracks* _tracks)
        : tracks(_tracks)
    {
        duk_eval_file_extra(ctx, "instruments/synth01/main.js");
        duk_pop(ctx);
    }

public:
    static App_View_Instrument* instance;

    static App_View_Instrument* getInstance(App_Tracks* _tracks)
    {
        if (!instance) {
            instance = new App_View_Instrument(_tracks);
        }
        return instance;
    }
};

App_View_Instrument* App_View_Instrument::instance = NULL;

#endif