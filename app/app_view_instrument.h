#ifndef APP_VIEW_INSTRUMENT_H_
#define APP_VIEW_INSTRUMENT_H_

// #include "./app_core_view.h"
#include "./app_tracks.h"
#include <PdBase.hpp>
#include <PdObject.h>
#include <app_core_renderer.h>
#include <app_core_view_js.h>

// #include <app_duk_extra.h>
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

        duk_push_c_function(ctx, App_View_Instrument::yoyo, DUK_VARARGS);
        duk_put_global_string(ctx, "yoyo");

        duk_eval_file_extra(ctx, "instruments/synth01/main.js");
        printf("1+2=%d\n", (int)duk_get_int(ctx, -1));

        printf("1+2=%d\n", (int)duk_get_global_string(ctx, "x"));
        printf("1+2=%s\n", duk_to_string(ctx, -1));
        printf("1+2=%d\n", duk_to_int(ctx, -1));
        duk_push_int(ctx, 10);
        printf("x=%d\n", duk_to_int(ctx, -1));

        duk_idx_t obj_idx;
        obj_idx = duk_push_object(ctx);
        duk_push_int(ctx, 123);
        duk_put_prop_string(ctx, obj_idx, "value");

        duk_put_global_string(ctx, "state");
        duk_get_global_string(ctx, "state");
        // duk_push_string(ctx, "1.2.3");
        duk_get_prop_string(ctx, -1, "value");
        printf("state.value=%s\n", duk_to_string(ctx, -1));

        duk_get_global_string(ctx, "processLine");
        duk_push_string(ctx, "foo bar");
        duk_call(ctx, 1);
        printf("encoded: %s\n", duk_to_string(ctx, -1));

        duk_get_global_string(ctx, "state");
        duk_get_prop_string(ctx, -1, "value");
        printf("state.valueA=%s\n", duk_to_string(ctx, -1));
        duk_get_prop_string(ctx, -1, "value2");
        printf("state.valueA2=%s\n", duk_to_string(ctx, -1));

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

    ~App_View_Instrument()
    {
        pd.closePatch(patch);
        pd.clear();
    }

    static duk_ret_t yoyo(duk_context* ctx)
    {
        duk_get_global_string(ctx, "state");
        // duk_get_prop_string(ctx, -1, "value");
        // duk_del_prop_string(ctx, -1, "value");
        // duk_push_string(ctx, "456");
        // duk_put_prop_string(ctx, -3, "value");
        // duk_replace(ctx, -1);
        // printf("stateC=%s\n", duk_to_string(ctx, -1));
        duk_push_int(ctx, 456);
        duk_put_prop_string(ctx, 0, "value");

        // duk_get_global_string(ctx, "state");
        // duk_push_int(ctx, 789);
        // duk_put_prop_string(ctx, -1, "value2");

        duk_get_global_string(ctx, "state");
        duk_get_prop_string(ctx, -1, "value");
        printf("state.valueB=%s\n", duk_to_string(ctx, -1));
        return 0;
    }
};

App_View_Instrument* App_View_Instrument::instance = NULL;

#endif