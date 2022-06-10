#ifndef APP_VIEW_TRACK_LOOP_H_
#define APP_VIEW_TRACK_LOOP_H_

#include "./app_display.h"
#include "./app_tracks.h"
#include "./app_view_table.h"

class App_View_TrackLoop : public App_View_Table<7, 6, 7> {
    // class App_View_TrackLoop : public App_View_Table<5, 4, 7> {
protected:
    App_Tracks* tracks;

public:
    App_View_TrackLoop(App_Tracks* _tracks)
        : App_View_Table(9)
        , tracks(_tracks)
    {
    }

    void renderCell(App_Display* display, uint16_t pos, uint16_t row, uint8_t col)
    {
        uint8_t note = naturalNotes[pos];
        if (cursor == pos) {
            display->setCursor(2, 1);
        }
        sprintf(display->text + strlen(display->text), "%c%s%d",
            tracks->looper->nextToPlay == note ? '>' : ' ', getNoteStr(note), getNoteOctave(note));
    }

    uint8_t update(UiKeys* keys, App_Display* display)
    {
        if (keys->B) {
            uint8_t note = naturalNotes[cursor];
            tracks->looper->on(note);
            render(display);
            return VIEW_CHANGED;
        }
        return App_View_Table::update(keys, display);
    }

    void colSeparator(App_Display* display) override
    {
    }
};

#endif