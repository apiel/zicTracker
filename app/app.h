#ifndef APP_H_
#define APP_H_

#include "./app_def.h"
#include "./app_display.h"
#include "./app_patterns.h"
#include "./app_tracks.h"
#include "./app_view_menu.h"
#include "./app_view_track.h"
#include "./app_view_trackLoop.h"
#include "./app_view_instrument.h"
#include <zic_seq_tempo.h>

class App {
public:
    Zic_Seq_Tempo<> tempo;
    App_Tracks tracks;

    App_Display display;
    UiKeys keys;

    App_View_Menu menuView;
    App_View_Track trackView;
    App_View_TrackLoop trackLoopView;
    App_View_Instrument instrumentView;

    App(App_Patterns* patterns)
        : tracks(patterns)
        , trackView(&tracks)
        , trackLoopView(&tracks)
        , instrumentView(&tracks)
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
        }
        return tracks.sample();
    }

    App_Display* render()
    {
        // TODO find a better place way to reset cursor
        display.cursorLen = 0;

        uint8_t view = menuView.getView();
        switch (view) {
        case VIEW_TRACK_LOOP:
            trackLoopView.render(&display);
            break;

        case VIEW_INSTRUMENT:
            instrumentView.render(&display);
            break;

        default:
            trackView.render(&display);
            break;
        }
        return &display;
    }

    App_Display* handleUi(uint8_t keysBin)
    {
        // TODO find a better place way to reset cursor
        display.cursorLen = 0;

        keys.Up = (keysBin >> UI_KEY_UP) & 1;
        keys.Down = (keysBin >> UI_KEY_DOWN) & 1;
        keys.Left = (keysBin >> UI_KEY_LEFT) & 1;
        keys.Right = (keysBin >> UI_KEY_RIGHT) & 1;
        keys.A = (keysBin >> UI_KEY_A) & 1;
        keys.B = (keysBin >> UI_KEY_B) & 1;
        // SDL_Log("%d%d%d%d%d%d\n", keys.Up, keys.Down, keys.Left, keys.Right, keys.A, keys.Y);

        if (menuView.update(&keys, &display)) {
        } else {
            bool rendered = false;
            uint8_t view = menuView.getView();
            switch (view) {
            case VIEW_TRACK:
                rendered = trackView.update(&keys, &display);
                break;
            case VIEW_TRACK_LOOP:
                rendered = trackLoopView.update(&keys, &display);
                break;
            case VIEW_INSTRUMENT:
                rendered = instrumentView.update(&keys, &display);
                break;
            }
            if (!rendered) {
                render();
            }
        }
        return &display;
    }
};

#endif