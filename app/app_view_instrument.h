#ifndef APP_VIEW_INSTRUMENT_H_
#define APP_VIEW_INSTRUMENT_H_

#include "./app_tracks.h"
#include <app_core_load_config.h>
#include <app_core_renderer.h>
#include <app_core_view_js.h>

#include "./app_view_grid.h"

#include <duktape.h>

class App_View_Instrument : public App_View_JS, App_Load_Config {
protected:
    App_Tracks* tracks;
    bool presetIsPlaying = true;
    char filepath[256];
    App_Audio_Track* track;
    App_State_Track* state;

    char* getFile(const char* extension)
    {
        sprintf(filepath, "instruments/pd/%s/main.%s", state->patchFilename, extension);
        return filepath;
    }

    App_View_Instrument()
    {
        tracks = App_Tracks::getInstance();
        loadPatch();
    }

public:
    static App_View_Instrument* instance;

    static App_View_Instrument* getInstance()
    {
        if (!instance) {
            instance = new App_View_Instrument();
        }
        return instance;
    }

    void loadPatch()
    {
        track = tracks->tracks[App_View_Grid::getTrackId()];
        state = &track->state[App_View_Grid::gridSelectedRow];

        if (!state->isPatchEmpty()) {
            duk_destroy_heap(ctx);
            if (!state->isPatchEmpty()) {
                loadContext();
                loadConfig(getFile("cfg"));

                duk_idx_t configIdx = duk_push_array(ctx);
                for (uint8_t i = 0; i < APP_CONFIG_SIZE && config[i][0] != 255; i++) {
                    duk_idx_t subArr_idx = duk_push_array(ctx);
                    if (config[i][0] == *(uint8_t*)"cc") {
                        duk_push_string(ctx, "cc");
                        duk_put_prop_index(ctx, subArr_idx, 0);
                        duk_push_int(ctx, config[i][1]);
                        duk_put_prop_index(ctx, subArr_idx, 1);
                        duk_push_int(ctx, config[i][2]);
                        duk_put_prop_index(ctx, subArr_idx, 2);
                        duk_push_int(ctx, config[i][3]);
                        duk_put_prop_index(ctx, subArr_idx, 3);
                        duk_put_prop_index(ctx, configIdx, i);
                    }
                }
                duk_put_global_string(ctx, "CONFIG");

                duk_push_int(ctx, VIEW_NONE);
                duk_put_global_string(ctx, "VIEW_NONE");
                duk_push_int(ctx, VIEW_CHANGED);
                duk_put_global_string(ctx, "VIEW_CHANGED");
                duk_push_int(ctx, VIEW_STATE_CHANGED);
                duk_put_global_string(ctx, "VIEW_STATE_CHANGED");

                duk_push_c_function(ctx, App_View_Instrument::duk_updateConfigCC, 3);
                duk_put_global_string(ctx, "updateConfigCC");

                duk_eval_file_extra(ctx, getFile("js"));
                duk_pop(ctx);
            }
        }
    }

    void focusView() override
    {
        loadPatch();
    }

    void preRender(App_Renderer* renderer) override
    {
        sprintf(renderer->text, "%26s %s\n", state->patchFilename, "A1");
        renderer->useColoredRow(0, COLOR_LIGHT);
        renderer->useColor(0, 27, COLOR_MEDIUM, 2);
    }

    void render(App_Renderer* renderer) override
    {
        if (state->isPatchEmpty()) {
            strcpy(renderer->text, "No patch selected\n");
        } else {
            App_View_JS::render(renderer);
        }
    }

    uint8_t update(UiKeys* keys, App_Renderer* renderer) override
    {
        // There should be a way to skip this from js...
        if (keys->Action) {
            track->noteOn(Zic::_NOTE_C3, 127, 0);
        } else if (track->isOn()) {
            track->noteOff(Zic::_NOTE_C3, 0);
        }

        return App_View_JS::update(keys, renderer);
    }

    static duk_ret_t duk_updateConfigCC(duk_context* ctx)
    {
        // if (current patch is playing)

        uint8_t cc = duk_get_int(ctx, 0);
        uint8_t val = duk_get_int(ctx, 1);
        uint8_t voice = duk_get_int(ctx, 2);
        // printf(">>>>>>>>>>>>> cc: %d, val: %d voice: %d\n", cc, val, voice);

        instance->tracks->cc(cc, val, voice);

        return 0;
    }

    // should we save from js or from patch / preset selector
    static duk_ret_t duk_save(duk_context* ctx)
    {
        instance->pullJsConfigAndSave();
        return 0;
    }

    void pullJsConfigAndSave()
    {
        duk_get_global_string(ctx, "CONFIG");
        void* ptr = duk_get_heapptr(ctx, -1);
        duk_size_t len = duk_get_length(ctx, -1);
        for (duk_size_t ii = 0; ii < len; ii++) {
            duk_push_heapptr(ctx, ptr);
            duk_get_prop_index(ctx, -1, ii);
            duk_get_prop_index(ctx, -1, 0);
            // printf("cmd: %s\n", duk_get_string(ctx, -1));
            if (*(uint8_t*)"cc" == *(uint8_t*)duk_get_string(ctx, -1)) {
                duk_get_prop_index(ctx, -2, 1);
                duk_get_prop_index(ctx, -3, 2);
                // printf("cc: %d, val: %d\n", duk_get_int(ctx, -2), duk_get_int(ctx, -1));
                for (uint8_t i = 0; i < APP_CONFIG_SIZE; i++) {
                    if (config[i][0] == *(uint8_t*)"cc" && config[i][1] == duk_get_int(ctx, -2)) {
                        config[i][2] = duk_get_int(ctx, -1);
                        break;
                    }
                }
            }
            duk_pop(ctx);
        }
        saveConfig(getFile("js"));
    }
};

App_View_Instrument* App_View_Instrument::instance = NULL;

#endif