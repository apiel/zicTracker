#ifndef APP_VIEW_TRACK_MASTER_H_
#define APP_VIEW_TRACK_MASTER_H_

#include <zic_seq_tempo.h>

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
protected:
    Zic_Seq_Tempo<>* tempo;

public:
    App_View_TrackMasterBpm(App_Tracks* _tracks, Zic_Seq_Tempo<>* _tempo)
        : App_View_TrackMasterRow(_tracks, "BPM ", 3)
        , tempo(_tempo)
    {
    }

    void renderValue(App_Display* display, uint8_t col)
    {
        sprintf(display->text + strlen(display->text), "%-3d", tempo->getBpm());
    }

    uint8_t update(UiKeys* keys, App_Display* display, uint8_t row, uint8_t col) override
    {
        if (keys->Right) {
            tempo->set(tempo->getBpm() + 1);
        } else if (keys->Up) {
            tempo->set(tempo->getBpm() + 10);
        } else if (keys->Left) {
            tempo->set(tempo->getBpm() - 1);
        } else if (keys->Down) {
            tempo->set(tempo->getBpm() - 10);
        }
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
    App_View_TrackMaster(App_Tracks* _tracks, Zic_Seq_Tempo<>* tempo)
        : App_View_Table(fields, VIEW_TRACK_MASTER_ROW, VIEW_TRACK_MASTER_COL)
        , bpmField(_tracks, tempo)
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