#ifndef APP_VIEW_TRACK_MASTER_H_
#define APP_VIEW_TRACK_MASTER_H_

#include "./app_display.h"
#include "./app_tracks.h"
#include "./app_view_table.h"

#define VIEW_TRACK_MASTER_ROW 1
#define VIEW_TRACK_MASTER_COL 2

class App_View_TrackMasterRow : public App_View_TableLabeledRow {
protected:
    App_Tracks* tracks;

public:
    App_View_TrackMasterRow(App_Tracks* _tracks, const char* _label, uint8_t _cursorLen = 5)
        : App_View_TableLabeledRow(_label, _cursorLen)
        , tracks(_tracks)
    {
    }
};

class App_View_TrackMasterBpm : public App_View_TrackMasterRow {
public:
    App_View_TrackMasterBpm(App_Tracks* _tracks)
        : App_View_TrackMasterRow(_tracks, "BPM ", 2)
    {
    }

    void renderValue(App_Display* display, uint8_t col)
    {
        strcat(display->text, "120");
        // sprintf(display->text + strlen(display->text), "%-2d", tracks->trackId + 1);
    }

    uint8_t update(UiKeys* keys, App_Display* display, uint8_t row, uint8_t col) override
    {
        // if (keys->Right || keys->Up) {
        //     tracks->select(tracks->trackId + 1);
        // } else if (keys->Left || keys->Down) {
        //     tracks->select(tracks->trackId - 1);
        // }
        return VIEW_CHANGED;
    }
};

class App_View_TrackMaster : public App_View_Table {
protected:
    App_View_TrackMasterBpm bpmField;

    App_View_TableField* fields[VIEW_TRACK_MASTER_ROW * VIEW_TRACK_MASTER_COL] = {
        // clang-format off
        &bpmField, &bpmField,
        // clang-format on
    };

public:
    App_View_TrackMaster(App_Tracks* _tracks)
        : App_View_Table(fields, VIEW_TRACK_MASTER_ROW, VIEW_TRACK_MASTER_COL)
        , bpmField(_tracks)
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