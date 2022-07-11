#ifndef APP_VIEW_TRACK_MASTER_H_
#define APP_VIEW_TRACK_MASTER_H_

#include "./app_display.h"
#include "./app_tracks.h"
#include "./app_view_table.h"

// TODO rename to track master
// where multiple tracks can be sent to it
// and where we can apply action for several tracks at once
//
// the keyboard should not be note but detune semitone value
// of 3 rows.
class App_View_TrackMaster : public App_View_Table<7, 6, 7> {
    // class App_View_TrackLoop : public App_View_Table<5, 4, 7> {
protected:
    App_Tracks* tracks;

public:
    App_View_TrackMaster(App_Tracks* _tracks)
        : App_View_Table(9)
        , tracks(_tracks)
    {
    }

    void renderCell(App_Display* display, uint16_t pos, uint16_t row, uint8_t col)
    {
        uint8_t note = Zic::naturalNotes[pos];
        if (cursor == pos) {
            display->setCursor(2, 1);
        }
        sprintf(display->text + strlen(display->text), "%c%s%d",
            tracks->looper->nextToPlay == note ? '>' : ' ', Zic::getNoteStr(note), Zic::getNoteOctave(note));
    }

    uint8_t update(UiKeys* keys, App_Display* display)
    {
        if (keys->A) {
            uint8_t note = Zic::naturalNotes[cursor];
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