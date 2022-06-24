#ifndef APP_VIEW_TRACK_H_
#define APP_VIEW_TRACK_H_

#include "./app_display.h"
#include "./app_tracks.h"
#include "./app_view_table.h"

#define VIEW_TRACK_COL 2

class App_View_Track : public App_View_Table<4, VIEW_TRACK_COL, 2> {
protected:
    App_Tracks* tracks;

public:
    App_View_Track(App_Tracks* _tracks)
        : App_View_Table(4)
        , tracks(_tracks)
    {
    }

    void startRow(App_Display* display, uint16_t row) override
    {
        sprintf(display->text + strlen(display->text), "%cTrack%d ", tracks->trackId == row ? '*' : ' ', row + 1);
    }

    void renderCell(App_Display* display, uint16_t pos, uint16_t row, uint8_t col)
    {
        // if (tracks->trackId == row) {
        //     display->setCursor(3);
        // }
        if (cursor == pos) {
            display->setCursor(3);
        }

        if (col == 0) {
            if (tracks->tracks[row]->looper.loopOn) {
                strcat(display->text, ">ON");
            } else {
                strcat(display->text, "OFF");
            }
        } else {
            // nextToPlay is wrong!
            sprintf(display->text + strlen(display->text), "%03d ", tracks->tracks[row]->looper.pattern->id + 1);
        }
    }

    uint8_t update(UiKeys* keys, App_Display* display)
    {
        uint8_t res = App_View_Table::update(keys, display);
        uint8_t id = cursor / VIEW_TRACK_COL;
        if (id != tracks->trackId) {
            tracks->select(id);
        }
        if (keys->A) {
            if (cursor % VIEW_TRACK_COL == 0) {
                tracks->looper->toggleLoopMode();
            }
        }
        return res;
    }
};

#endif