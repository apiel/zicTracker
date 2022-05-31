#ifndef APP_H_
#define APP_H_

#include "./app_def.h"
#include "./app_tracks.h"
#include <zic_seq_tempo.h>

#define APP_MENU_SIZE 7

typedef struct {
    const char* name;
    const char key;
    bool isBase = false;
} Menu;

class App {
public:
    Zic_Seq_Tempo<> tempo;
    App_Tracks tracks;

    char display[128];
    UiKeys keys;

    Menu menu[APP_MENU_SIZE] = {
        { "Track", 'T', true },
        { "Track Loop", 'L' },
        { "Track pattern", 'P' },
        { "Pattern", 'P', true },
        { "Pattern edit", 'E' },
        { "Instrument", 'I', true },
        { "Instrument edit", 'E' },
    };
    uint8_t currentMenu = 0;

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

        if (keys.A) {
            strcpy(display, "");
            for (uint8_t i = 0; i < APP_MENU_SIZE; i++) {
                if (i != 0 && menu[i].isBase) {
                    strcat(display, "\n");
                }
                if (i == currentMenu) {
                    sprintf(display + strlen(display), "~1%c~0 ", menu[i].key);
                } else {
                    sprintf(display + strlen(display), "%c ", menu[i].key);
                }
            }
            sprintf(display + strlen(display), "\n\n%s", menu[currentMenu].name);
        } else {
            sprintf(display, "%d%d%d%d%d%d", keys.Up, keys.Down, keys.Left, keys.Right, keys.A, keys.B);
        }
        return display;
    }
};

#endif