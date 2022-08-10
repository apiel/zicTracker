#ifndef APP_VIEW_TRACK_SEQUENCER_H_
#define APP_VIEW_TRACK_SEQUENCER_H_

#include "./app_display.h"
#include "./app_tracks.h"
#include "./app_view_table.h"

#define VIEW_TRACK_SEQUENCER_ROW 2
#define VIEW_TRACK_SEQUENCER_COL (3 * TRACK_COUNT)

class App_View_TrackSequencerHeader : public App_View_TableField {
protected:
    const static uint8_t headerCount = 3;
    const char* headers[headerCount] = { "PAT ", "ACT ", "DET " };

public:
    void render(App_Display* display, uint8_t row, uint8_t col, uint8_t selectedRow, uint8_t selectedCol)
    {
        if (row == 0) {
            // sprintf(display->text + strlen(display->text), "%cTR%d", tracks->trackId == trackId ? '*' : ' ', trackId + 1);
            sprintf(display->text + strlen(display->text), "TRACK %d     ", col + 1);
        } else {
            strcat(display->text, headers[col % headerCount]);
        }
    }
};

class App_View_TrackSequencer : public App_View_Table {
protected:
    App_View_TrackSequencerHeader header;

    App_View_TableField* fields[VIEW_TRACK_SEQUENCER_ROW * VIEW_TRACK_SEQUENCER_COL] = {
        // clang-format off
        &header, NULL, NULL, &header, NULL, NULL, &header, NULL, NULL, &header, NULL, NULL,
        &header, &header, &header, &header, &header, &header, &header, &header, &header, &header, &header, &header,
        // clang-format on
    };

public:
    App_View_TrackSequencer(App_Tracks* _tracks)
        : App_View_Table(fields, VIEW_TRACK_SEQUENCER_ROW, VIEW_TRACK_SEQUENCER_COL)
    {
        initSelection();
    }

    void initDisplay(App_Display* display)
    {
        display->useColoredHeader(1);
        App_View_Table::initDisplay(display);
    }
};

#endif