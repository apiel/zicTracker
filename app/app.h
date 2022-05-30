#ifndef APP_H_
#define APP_H_

#include "./app_tracks.h"
#include <zic_seq_tempo.h>

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

    char * ui(uint8_t keys) {
        return display;
    }
};

#endif