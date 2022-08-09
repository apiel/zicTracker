#ifndef APP_VIEW_TRACK_DELAY_H_
#define APP_VIEW_TRACK_DELAY_H_

#include "./app_display.h"
#include "./app_tracks.h"
#include "./app_view_table.h"

#define VIEW_TRACK_DELAY_ROW 8
#define VIEW_TRACK_DELAY_COL 4
#define VIEW_TRACK_DELAY_ROW_HEADERS 3

// TODO
// make a way to save delay settings
// to be able to load settings
// might add filter to make reverb...
// might add lfo to make chorus...

class App_View_TrackDelaySelect : public App_View_TableLabeledRow {
protected:
    App_Tracks* tracks;

public:
    App_View_TrackDelaySelect(App_Tracks* _tracks)
        : App_View_TableLabeledRow("Track ", 2)
        , tracks(_tracks)
    {
    }

    void renderValue(App_Display* display, uint8_t col)
    {
        sprintf(display->text + strlen(display->text), "%-2d", tracks->trackId + 1);
    }

    uint8_t update(UiKeys* keys, App_Display* display, uint8_t row, uint8_t col) override
    {
        if (keys->Right || keys->Up) {
            tracks->select(tracks->trackId + 1);
        } else if (keys->Left || keys->Down) {
            tracks->select(tracks->trackId - 1);
        }
        return VIEW_CHANGED;
    }
};

class App_View_TrackDelayHeader : public App_View_TableField {
protected:
    const char* headers[4] = { "  ", "SEC. ", "AMP ", "FBK" };

public:
    bool isSelectable(uint8_t row, uint8_t col) override
    {
        return false;
    }

    void render(App_Display* display, uint8_t row, uint8_t col, uint8_t selectedRow, uint8_t selectedCol)
    {
        strcat(display->text, headers[col]);
    }
};

class App_View_TrackDelaySetting : public App_View_TableField {
protected:
    App_Tracks* tracks;

public:
    App_View_TrackDelaySetting(App_Tracks* _tracks)
        : tracks(_tracks)
    {
    }

    bool isSelectable(uint8_t row, uint8_t col) override
    {
        return col != 0;
    }

    void render(App_Display* display, uint8_t row, uint8_t col, uint8_t selectedRow, uint8_t selectedCol)
    {
        uint8_t cursorLen[VIEW_TRACK_DELAY_COL] = { 0, 4, 3, 3 };
        if (selectedRow == row && selectedCol == col) {
            display->setCursor(cursorLen[col]);
        }
        uint8_t delayPos = row - VIEW_TRACK_DELAY_ROW_HEADERS;
        // Zic_Seq_Step* step = &patterns->patterns[*currentPatternId].steps[stepPos];
        Zic_Effect_Delay* delay = tracks->track->delays[delayPos];
        switch (col) {
        case 0:
            sprintf(display->text + strlen(display->text), "%d ", delayPos + 1);
            break;

        case 1:
            sprintf(display->text + strlen(display->text), "%.2f ", delay->sec);
            break;

        case 2:
            if (delay->amplitude == 1.0f) {
                strcat(display->text, "1.00 ");
            } else {
                sprintf(display->text + strlen(display->text), ".%02d ", (uint8_t)(delay->amplitude * 100));
            }
            break;

        case 3:
            if (delay->feedback == 1.0f) {
                strcat(display->text, "1.00 ");
            } else {
                sprintf(display->text + strlen(display->text), ".%02d ", (uint8_t)(delay->feedback * 100));
            }
            break;
        }
    }

    uint8_t update(UiKeys* keys, App_Display* display, uint8_t row, uint8_t col)
    {
        // Zic_Seq_Step* step = &patterns->patterns[*currentPatternId].steps[row - VIEW_PATTERN_ROW_HEADERS];
        // if (col == 4) {
        //     step->slide = !step->slide;
        // } else {
        //     int8_t directions[] = { 0, 1, 12, 10 };
        //     int8_t direction = 0;
        //     if (keys->Right) {
        //         direction = 1;
        //     } else if (keys->Left) {
        //         direction = -1;
        //     } else if (keys->Up) {
        //         direction = directions[col];
        //     } else if (keys->Down) {
        //         direction = -directions[col];
        //     }
        //     switch (col) {
        //     case 1:
        //         step->instrument = (step->instrument + INSTRUMENT_COUNT + direction) % INSTRUMENT_COUNT;
        //         break;
        //     case 2: {
        //         uint8_t note = range(step->note + direction, Zic::_NOTE_START - 1, Zic::_NOTE_END);
        //         step->note = range(note, Zic::_NOTE_START, Zic::_NOTE_END) != note ? 0 : note;
        //         break;
        //     }
        //     case 3:
        //         step->velocity = range(step->velocity + direction, 0, 127);
        //         break;
        //     }
        // }
        return VIEW_CHANGED;
    }
};

class App_View_TrackDelay : public App_View_Table {
protected:
    // App_Tracks* tracks;

    App_View_TrackDelaySelect selectField;
    App_View_TrackDelayHeader headerField;
    App_View_TrackDelaySetting settingField;

    App_View_TableField* fields[VIEW_TRACK_DELAY_ROW * VIEW_TRACK_DELAY_COL] = {
        // clang-format off
        &selectField, &selectField, NULL, NULL,
        NULL, NULL, NULL, NULL,
        &headerField, &headerField, &headerField, &headerField,
        &settingField, &settingField, &settingField, &settingField,
        &settingField, &settingField, &settingField, &settingField,
        &settingField, &settingField, &settingField, &settingField,
        &settingField, &settingField, &settingField, &settingField,
        &settingField, &settingField, &settingField, &settingField,
        // clang-format on
    };

public:
    App_View_TrackDelay(App_Tracks* _tracks)
        : App_View_Table(fields, VIEW_TRACK_DELAY_ROW, VIEW_TRACK_DELAY_COL)
        // , tracks(_tracks)
        , selectField(_tracks)
        , settingField(_tracks)
    {
        initSelection();
    }

    void initDisplay(App_Display* display)
    {
        display->useColoredLabel();
        display->useColoredHeader(2);
        App_View_Table::initDisplay(display);
    }
};

#endif