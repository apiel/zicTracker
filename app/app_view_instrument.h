#ifndef APP_VIEW_INSTRUMENT_H_
#define APP_VIEW_INSTRUMENT_H_

#include "./app_tracks.h"
#include <PdBase.hpp>
#include <PdObject.h>
#include <app_core_renderer.h>
#include <app_core_view_js.h>

#include <duktape.h>

#define VIEW_INSTR_ROW 8
#define VIEW_INSTR_COL 3
#define VIEW_INSTR_LABELS 7

class App_View_Instrument : public App_View_JS {
protected:
    App_Tracks* tracks;

    PdObject pdObject;
    pd::PdBase pd;
    pd::Patch patch;

public:
    App_View_Instrument(App_Tracks* _tracks)
        : tracks(_tracks)
        , pdObject(255)
    {
        if (!pd.init(0, APP_CHANNELS, SAMPLE_RATE)) {
            APP_LOG("Could not init pd\n");
        }
        // pd.computeAudio(true);
        patch = pd.openPatch("main.pd", "instruments/synth01");
        pd.setReceiver(&pdObject);
        // pd.setMidiReceiver(&pdObject);
        pd.sendControlChange(1, 1, 10);
        yo();
    }

    ~App_View_Instrument()
    {
        pd.closePatch(patch);
        pd.clear();
    }

    void yo(void)
    {
        duk_context* ctx = duk_create_heap_default();
        int ret = duk_peval_file(ctx, "instruments/synth01/main.js");
        printf("1+2=%d\n", (int)duk_get_int(ctx, -1));
        duk_destroy_heap(ctx);
    }
};

#endif