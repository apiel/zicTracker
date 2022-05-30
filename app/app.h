#ifndef APP_H_
#define APP_H_

#include "./app_tracks.h"
#include <zic_seq_tempo.h>

typedef struct {
    bool keyUp = false;
    bool keyDown = false;
    bool keyRight = false;
    bool keyLeft = false;
    bool keyA = false;
    bool keyB = false;
    bool keyX = false;
    bool keyY = false;
} UiKey;

class App {
public:
    Zic_Seq_Tempo<> tempo;
    App_Tracks tracks;

    char display[128];

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

    char* handleUi(uint8_t keys)
    {
        printf("%d%d%d%d%d%d",
            ui.keyUp, ui.keyDown, ui.keyLeft, ui.keyRight, ui.keyA, ui.keyY);
        return display;
    }
};

#endif