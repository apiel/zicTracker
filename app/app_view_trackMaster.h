#ifndef APP_VIEW_TRACK_MASTER_H_
#define APP_VIEW_TRACK_MASTER_H_

#include "./app_display.h"
#include "./app_tracks.h"
#include "./app_view.h"

class App_View_TrackMaster : public App_View {
protected:
    App_Tracks* tracks;

public:
    App_View_TrackMaster(App_Tracks* _tracks)
        : tracks(_tracks)
    {
    }

    void render(App_Display* display)
    {
        strcpy(display->text, "Master track\n\nto be implemented");
    }

    uint8_t update(UiKeys* keys, App_Display* display)
    {
        return 0;
    }
};

#endif