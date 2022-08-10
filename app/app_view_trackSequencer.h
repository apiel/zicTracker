#ifndef APP_VIEW_TRACK_SEQUENCER_H_
#define APP_VIEW_TRACK_SEQUENCER_H_

#include "./app_display.h"
#include "./app_tracks.h"
#include "./app_view_table.h"

#define VIEW_TRACK_SEQUENCER_ROW 2
#define VIEW_TRACK_SEQUENCER_COL 2

class App_View_TrackSequencerRow : public App_View_TableLabeledRow {
protected:
    App_Tracks* tracks;

public:
    App_View_TrackSequencerRow(App_Tracks* _tracks, const char* _label, uint8_t _cursorLen = 5)
        : App_View_TableLabeledRow(_label, _cursorLen)
        , tracks(_tracks)
    {
    }
};

class App_View_TrackSequencerAllOff : public App_View_TrackSequencerRow {
public:
    App_View_TrackSequencerAllOff(App_Tracks* _tracks)
        : App_View_TrackSequencerRow(_tracks, "All ", 3)
    {
    }

    void renderValue(App_Display* display, uint8_t col)
    {
        strcat(display->text, "OFF");
    }

    uint8_t update(UiKeys* keys, App_Display* display, uint8_t row, uint8_t col) override
    {
        for (uint8_t i = 0; i < TRACK_COUNT; i++) {
            tracks->tracks[i]->looper.nextState.stop();
        }
        return VIEW_CHANGED;
    }
};

class App_View_TrackSequencer : public App_View_Table {
protected:
    App_View_TrackSequencerAllOff allOffField;

    App_View_TableField* fields[VIEW_TRACK_SEQUENCER_ROW * VIEW_TRACK_SEQUENCER_COL] = {
        // clang-format off
        NULL, NULL,
        &allOffField, &allOffField,
        // clang-format on
    };

public:
    App_View_TrackSequencer(App_Tracks* _tracks)
        : App_View_Table(VIEW_TRACK_SEQUENCER, fields, VIEW_TRACK_SEQUENCER_ROW, VIEW_TRACK_SEQUENCER_COL)
        , allOffField(_tracks)
    {
        initSelection();
    }

    void initDisplay(App_Display* display)
    {
        display->useColoredLabel();
        App_View_Table::initDisplay(display);
    }
};

#endif