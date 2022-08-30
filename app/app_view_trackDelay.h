#ifndef APP_VIEW_TRACK_DELAY_H_
#define APP_VIEW_TRACK_DELAY_H_

#include "./app_renderer.h"
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

// TODO improve delay audio

class App_View_TrackDelaySelect : public App_View_TableLabeledRow {
protected:
    App_Tracks* tracks;

public:
    App_View_TrackDelaySelect(App_Tracks* _tracks)
        : App_View_TableLabeledRow("Track ", 1)
        , tracks(_tracks)
    {
    }

    void renderValue(App_Renderer* display, uint8_t col)
    {
        sprintf(display->text + strlen(display->text), "%-2d", tracks->trackId + 1);
    }

    uint8_t update(UiKeys* keys, App_Renderer* display, uint8_t row, uint8_t col) override
    {
        if (keys->Right || keys->Up) {
            tracks->select(tracks->trackId + 1);
        } else if (keys->Left || keys->Down) {
            tracks->select(tracks->trackId - 1);
        }
        return VIEW_CHANGED;
    }
};

class App_View_TrackDelayOnOff : public App_View_TableFieldCursor {
protected:
    App_Tracks* tracks;

public:
    App_View_TrackDelayOnOff(App_Tracks* _tracks)
        : App_View_TableFieldCursor(3)
        , tracks(_tracks)
    {
    }

    void renderValue(App_Renderer* display, uint8_t col)
    {
        if (tracks->track->delayEnabled) {
            strcat(display->text, "ON ");
        } else {
            strcat(display->text, "OFF");
        }
    }

    uint8_t update(UiKeys* keys, App_Renderer* display, uint8_t row, uint8_t col) override
    {
        tracks->track->toggleDelay();
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

    void render(App_Renderer* display, uint8_t row, uint8_t col, uint8_t selectedRow, uint8_t selectedCol)
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

    void render(App_Renderer* display, uint8_t row, uint8_t col, uint8_t selectedRow, uint8_t selectedCol)
    {
        uint8_t cursorLen[VIEW_TRACK_DELAY_COL] = { 0, 4, 3, 3 };
        if (selectedRow == row && selectedCol == col) {
            display->setCursor(cursorLen[col]);
        }
        uint8_t delayPos = row - VIEW_TRACK_DELAY_ROW_HEADERS;
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
                strcat(display->text, "1.0 ");
            } else {
                sprintf(display->text + strlen(display->text), ".%02d ", (uint8_t)(delay->amplitude * 100));
            }
            break;

        case 3:
            if (delay->feedback == 1.0f) {
                strcat(display->text, "1.0 ");
            } else {
                sprintf(display->text + strlen(display->text), ".%02d ", (uint8_t)(delay->feedback * 100));
            }
            break;
        }
    }

    uint8_t update(UiKeys* keys, App_Renderer* display, uint8_t row, uint8_t col)
    {
        Zic_Effect_Delay* delay = tracks->track->delays[row - VIEW_TRACK_DELAY_ROW_HEADERS];

        float direction = 0;
        if (keys->Right) {
            direction = 0.01;
        } else if (keys->Left) {
            direction = -0.01;
        } else if (keys->Up) {
            direction = 0.10;
        } else if (keys->Down) {
            direction = -0.10;
        }
        switch (col) {
        case 1:
            delay->setSec(delay->sec + direction);
            break;
        case 2: {
            delay->setAmplitude(delay->amplitude + direction);
            break;
        }
        case 3:
            delay->setFeedback(delay->feedback + direction);
            break;
        }
        return VIEW_CHANGED;
    }
};

class App_View_TrackDelay : public App_View_Table {
protected:
    // App_Tracks* tracks;

    App_View_TrackDelaySelect selectField;
    App_View_TrackDelayOnOff onOffField;
    App_View_TrackDelayHeader headerField;
    App_View_TrackDelaySetting settingField;

    App_View_TableField* fields[VIEW_TRACK_DELAY_ROW * VIEW_TRACK_DELAY_COL] = {
        // clang-format off
        &selectField, &selectField, &onOffField, NULL,
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
        , onOffField(_tracks)
        , settingField(_tracks)
    {
        initSelection();
    }

    void initDisplay(App_Renderer* display)
    {
        display->useColoredLabel();
        display->useColoredHeader(2);
        App_View_Table::initDisplay(display);
    }
};

#endif