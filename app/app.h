#ifndef APP_H_
#define APP_H_

#include "./app_def.h"
#include "./app_tracks.h"
#include "./app_view_menu.h"
#include <zic_seq_tempo.h>

class App {
public:
    Zic_Seq_Tempo<> tempo;
    App_Tracks tracks;

    char display[128];
    UiKeys keys;

    App_View_Menu menuView;

    void start()
    {
        tracks.looper->setLoopMode(true);
        tracks.looper->on(60);
    }

    int16_t sample()
    {
        if (tempo.next()) {
            tracks.next();
        }
        return tracks.sample();
    }

    char* handleUi(uint8_t keysBin)
    {
        keys.Up = (keysBin >> UI_KEY_UP) & 1;
        keys.Down = (keysBin >> UI_KEY_DOWN) & 1;
        keys.Left = (keysBin >> UI_KEY_LEFT) & 1;
        keys.Right = (keysBin >> UI_KEY_RIGHT) & 1;
        keys.A = (keysBin >> UI_KEY_A) & 1;
        keys.B = (keysBin >> UI_KEY_B) & 1;
        // SDL_Log("%d%d%d%d%d%d\n", keys.Up, keys.Down, keys.Left, keys.Right, keys.A, keys.Y);

        if (menuView.render(&keys, *display)) {
        } else {
            sprintf(display, "%d%d%d%d%d%d", keys.Up, keys.Down, keys.Left, keys.Right, keys.A, keys.B);
        }
        return display;
    }
};

#endif