#ifndef APP_VIEW_TRACK_LOOP_H_
#define APP_VIEW_TRACK_LOOP_H_

#include "./app_view.h"

class App_View_TrackLoop : App_View {
public:
    void render(char* display)
    {
        sprintf(display, "Loooooooper");
    }

    uint8_t update(UiKeys* keys, char* display)
    {
        return VIEW_NONE;
    }
};

#endif