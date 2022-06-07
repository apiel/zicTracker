#ifndef APP_VIEW_TRACK_LOOP_H_
#define APP_VIEW_TRACK_LOOP_H_

#include "./app_tracks.h"
#include "./app_view_table.h"

class App_View_TrackLoop : public App_View_Table<7, 6, 7> {
protected:
    App_Tracks* tracks;

public:
    App_View_TrackLoop(App_Tracks* _tracks)
        : App_View_Table(9)
        , tracks(_tracks)
    {
    }

    void renderCell(Display* display, uint16_t pos, uint16_t row, uint8_t col)
    {
        char octave[1];
        uint8_t note = naturalNotes[pos];
        strcat(display->text, tracks->looper->nextToPlay == note ? ">" : " ");
        if (cursor == pos) {
            strcat(display->text, "[");
        }
        strcat(display->text, getNoteStr(note));
        octave[0] = '0' + getNoteOctave(note);
        strcat(display->text, octave);
    }

    uint8_t update(UiKeys* keys, Display* display)
    {
        if (keys->B) {
            uint8_t note = naturalNotes[cursor];
            tracks->looper->on(note);
            render(display);
            return VIEW_CHANGED;
        }
        return App_View_Table::update(keys, display);
    }

    void colSeparator(Display* display) override
    {
    }
};

#endif