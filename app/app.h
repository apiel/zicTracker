#ifndef APP_H_
#define APP_H_

#include "./app_def.h"
#include "./app_display.h"
#include "./app_patterns.h"
#include "./app_tracks.h"
#include "./app_view_instrument.h"
#include "./app_view_menu.h"
#include "./app_view_pattern.h"
#include "./app_view_project.h"
#include "./app_view_track.h"
#include "./app_view_trackDelay.h"
#include "./app_view_trackSequencer.h"
#include <zic_seq_tempo.h>

class App {
public:
    Zic_Seq_Tempo<> tempo;
    App_Tracks tracks;

    App_Display* display;
    UiKeys keys;

    App_View_Track trackView;
    App_View_TrackSequencer trackSeqView;
    App_View_Instrument instrumentView;
    App_View_Pattern patternView;
    App_View_TrackDelay trackDelayView;
    App_View_Project projectView;

    App_View_Menu menuView;

    bool rendered = false;

#define APP_MENU_SIZE 8
    Menu menu[APP_MENU_SIZE] = {
        (Menu) { "Tracks sequencer", 'S', &trackSeqView, 'T', true },
        (Menu) { "Tracks", 'T', &trackView, 'T', false },
        (Menu) { "Track delay", 'D', &trackDelayView, 'T', false },
        (Menu) { "Instruments", 'I', &instrumentView, 'I', true },
        (Menu) { "Instruments kit", 'K', NULL, 'I', false }, // this is how to save a kit
        // 4 LFO -> can be assigned to any changeable values and can be use for multiple instrument at the same time
        // 4 Extra Envelop -> same as LFO
        // IFX: reverb, distortion...
        // { "Project", 'P', VIEW_TRACK_PROJECT, 'T', false }, // Select project
        (Menu) { "Pattern", 'P', &patternView, 'P', true },
        (Menu) { "Sampler", 'S', NULL, 'S', true }, // Record all track to sample and edit sample
        (Menu) { "Project", 'P', &projectView, 'P', true }, // Select project
    };

    App(App_Patterns* patterns, App_Display* _display)
        : tracks(patterns)
        , display(_display)
        , trackView(&tracks)
        , trackSeqView(&tracks)
        , instrumentView(&tracks)
        , patternView(patterns)
        , trackDelayView(&tracks)
        , projectView(&tempo)
        , menuView(&menu[0], APP_MENU_SIZE)
    {
    }

    void start()
    {
        // tracks.looper->setLoopMode(true);
        // tracks.looper->on(60);
    }

    int16_t sample()
    {
        if (tempo.next()) {
            tracks.next();
            if (menuView.getView()->renderOn(EVENT_VIEW_ON_TEMPO)) {
                render();
            }
        }
        return tracks.sample();
    }

    void render()
    {
        // TODO find a better place way to reset cursor
        display->reset();
        menuView.getView()->render(display);
        display->drawText();
        rendered = true;
    }

    void handleUi(uint8_t keysBin)
    {
        keys.Up = (keysBin >> UI_KEY_UP) & 1;
        keys.Down = (keysBin >> UI_KEY_DOWN) & 1;
        keys.Left = (keysBin >> UI_KEY_LEFT) & 1;
        keys.Right = (keysBin >> UI_KEY_RIGHT) & 1;
        keys.Edit = (keysBin >> UI_KEY_EDIT) & 1;
        keys.Menu = (keysBin >> UI_KEY_MENU) & 1;
        // SDL_Log("%d%d%d%d%d%d\n", keys.Up, keys.Down, keys.Left, keys.Right, keys.A, keys.Y);

        if (keys.Menu && keys.Edit) {
            for (uint8_t i = 0; i < TRACK_COUNT; i++) {
                tracks.tracks[i]->looper.togglePlay();
            }
        } else if (menuView.update(&keys, display) != VIEW_NONE) {
            render();
        } else if (menuView.getView()->update(&keys, display) != VIEW_NONE) {
            render();
        }
    }
};

#endif