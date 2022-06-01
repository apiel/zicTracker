#ifndef APP_VIEW_TRACK_LOOP_H_
#define APP_VIEW_TRACK_LOOP_H_

#include "./app_tracks.h"
#include "./app_view_table.h"

class App_View_TrackLoop : public App_View_Table<5, 4, 7> {
protected:
    App_Tracks* tracks;

public:
    App_View_TrackLoop(App_Tracks* _tracks)
        : App_View_Table(9)
        , tracks(_tracks)
    {
    }

    void renderPos(char* display, uint16_t _pos)
    {
        char octave[1];
        uint8_t note = naturalNotes[_pos];
        bool colored = false;
        if (cursor == _pos) {
            strcat(display, "~b");
            colored = true;
        }
        if (tracks->looper->play == note) {
            strcat(display, "~1");
            colored = true;
        }
        strcat(display, getNoteStr(note));
        octave[0] = '0' + getNoteOctave(note);
        strcat(display, octave);
        if (colored) {
            strcat(display, "~0");
        }
    }

    uint8_t update(UiKeys* keys, char* display)
    {
        if (keys->B) {
            uint8_t note = naturalNotes[cursor];
            tracks->looper->on(note);
        } else {
            App_View_Table::update(keys, display);
        }
    }
};

#endif