#ifndef APP_VIEW_INSTRUMENT_H_
#define APP_VIEW_INSTRUMENT_H_

#include "./app_tracks.h"
#include <app_core_load_config.h>
#include <app_core_renderer.h>
#include <app_core_view_js.h>

#include <duktape.h>

#define VIEW_INSTR_ROW 8
#define VIEW_INSTR_COL 3
#define VIEW_INSTR_LABELS 7

class App_View_Instrument : public App_View_JS, App_Load_Config {
protected:
    App_Tracks* tracks;

    const char* getConfigFile()
    {
        return "instruments/synth01/main.cfg";
    }

    App_View_Instrument(App_Tracks* _tracks)
        : tracks(_tracks)
    {
        load();
        // logCfg();

        duk_idx_t arr_idx = duk_push_array(ctx);
        for (int i = 0; i < APP_CONFIG_SIZE && config[i][0] != 255; i++) {
            duk_idx_t subArr_idx = duk_push_array(ctx);
            if (config[i][0] == *(uint8_t*)"cc") {
                duk_push_string(ctx, "cc");
                duk_put_prop_index(ctx, subArr_idx, 0);
                duk_push_int(ctx, config[i][1]);
                duk_put_prop_index(ctx, subArr_idx, 1);
                duk_push_int(ctx, config[i][2]);
                duk_put_prop_index(ctx, subArr_idx, 2);
                duk_put_prop_index(ctx, arr_idx, i);
            }
        }
        duk_put_global_string(ctx, "CONFIG");

        duk_push_int(ctx, VIEW_NONE);
        duk_put_global_string(ctx, "VIEW_NONE");
        duk_push_int(ctx, VIEW_CHANGED);
        duk_put_global_string(ctx, "VIEW_CHANGED");
        duk_push_int(ctx, VIEW_STATE_CHANGED);
        duk_put_global_string(ctx, "VIEW_STATE_CHANGED");

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