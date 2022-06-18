#ifndef APP_VIEW_TRACK_PATTERN_H_
#define APP_VIEW_TRACK_PATTERN_H_

#include "./app_display.h"
#include "./app_tracks.h"
#include "./app_view_table.h"

// There will be 6x6 pattern in 6 bank, so a total of 256 for a single project
//              1 - 36      A,B,C,D,E,F                   no project limit but must be loaded from SD

class App_View_TrackPattern : public App_View_Table<7, 6, 6> {
protected:
    App_Tracks* tracks;

public:
    App_View_TrackPattern(App_Tracks* _tracks)
        : App_View_Table(7)
        , tracks(_tracks)
    {
    }

    void renderCell(App_Display* display, uint16_t pos, uint16_t row, uint8_t col)
    {
        if (cursor == pos) {
            display->setCursor(2, 1);
        }

        if (row == 0) {
            sprintf(display->text + strlen(display->text), "%c%c ", col == 2 ? '*': ' ', 'A' + col);
        } else {
            sprintf(display->text + strlen(display->text), " %02d", pos - 5);
        }
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