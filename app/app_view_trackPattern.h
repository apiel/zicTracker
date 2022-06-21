#ifndef APP_VIEW_TRACK_PATTERN_H_
#define APP_VIEW_TRACK_PATTERN_H_

#include "./app_display.h"
#include "./app_tracks.h"
#include "./app_view_table.h"

#define TRACK_PATTERN_COL 5
class App_View_TrackPattern : public App_View_Table<7, TRACK_PATTERN_COL, 5> {
protected:
    App_Tracks* tracks;

public:
    App_View_TrackPattern(App_Tracks* _tracks)
        : App_View_Table(PATTERN_COUNT / TRACK_PATTERN_COL)
        , tracks(_tracks)
    {
    }

    void renderCell(App_Display* display, uint16_t pos, uint16_t row, uint8_t col)
    {
        if (cursor == pos) {
            display->setCursor(3, 1);
        }
        sprintf(display->text + strlen(display->text), " %03d", pos + 1);
    }

    void colSeparator(App_Display* display) override
    {
    }

    uint8_t update(UiKeys* keys, App_Display* display)
    {
        uint8_t res = App_View_Table::update(keys, display);
        // uint8_t id = cursor / VIEW_TRACK_COL;
        // if (id != tracks->trackId) {
        //     tracks->select(id);
        // }
        // if (keys->A) {
        //     if (cursor % VIEW_TRACK_COL == 0) {
        //         tracks->looper->toggleLoopMode();
        //     }
        // }
        return res;
    }
};

#endif