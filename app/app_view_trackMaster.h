#ifndef APP_VIEW_TRACK_MASTER_H_
#define APP_VIEW_TRACK_MASTER_H_

#include "./app_display.h"
#include "./app_tracks.h"
#include "./app_view_table.h"

class App_View_TrackMaster : public App_View_Table<7, 6, 6> {
protected:
    App_Tracks* tracks;

public:
    App_View_TrackMaster(App_Tracks* _tracks)
        : App_View_Table(7)
        , tracks(_tracks)
    {
    }

void initDisplay(App_Display* display) override
    {
        strcpy(display->text, "Master track\n\nto be implemented");
    }

    void renderCell(App_Display* display, uint16_t pos, uint16_t row, uint8_t col)
    {
        // TODO there is no point to have this GRID.
        // Let just keep it simple with the same UI as for track
        // int8_t detune = pos - 24;
        // if (cursor == pos) {
        //     display->setCursor(3);
        // }
        // sprintf(display->text + strlen(display->text), "%c%02d ", detune < 0 ? '-' : '+', abs(detune));
        // sprintf(display->text + strlen(display->text), "%c%s%d",
        //     tracks->looper->nextState.detune == note ? '>' : ' ', Zic::getNoteStr(note), Zic::getNoteOctave(note));
    }

    uint8_t update(UiKeys* keys, App_Display* display)
    {
        if (keys->A) {
            // uint8_t note = Zic::naturalNotes[cursor];
            // tracks->looper->nextState.set(note);
            // render(display);
            // return VIEW_CHANGED;
        }
        return App_View_Table::update(keys, display);
    }

    void colSeparator(App_Display* display) override
    {
    }
};

#endif