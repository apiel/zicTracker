#ifndef APP_VIEW_TRACK_H_
#define APP_VIEW_TRACK_H_

#include "./app_display_base.h"
#include "./app_tracks.h"
#include "./app_view_table.h"

#include <zic_seq_pattern.h>

#define VIEW_TRACK_ROW 3
#define VIEW_TRACK_COL 5

class App_View_TrackRow : public App_View_TableField {
protected:
    App_Tracks* tracks;
    const char* label;

public:
    App_View_TrackRow(App_Tracks* _tracks, const char* _label)
        : tracks(_tracks)
        , label(_label)
    {
    }

    virtual void renderValue(App_Display_Base* display, uint8_t trackId, App_Audio_Track* track) = 0;

    bool isSelectable(uint8_t row, uint8_t col) override
    {
        return col != 0 && row != 0;
    }

    void render(App_Display_Base* display, uint8_t row, uint8_t col, uint8_t selectedRow, uint8_t selectedCol)
    {
        if (col == 0) {
            strcat(display->text, label);
        } else {
            if (selectedRow == row && selectedCol == col) {
                display->setCursor(3, 1);
            }
            uint8_t trackId = col - 1;
            App_Audio_Track* track = tracks->tracks[trackId];
            renderValue(display, trackId, track);
        }
    }

    virtual uint8_t update(UiKeys* keys, App_Display_Base* display, App_Audio_Track* track)
    {
        return VIEW_NONE;
    }

    uint8_t update(UiKeys* keys, App_Display_Base* display, uint8_t row, uint8_t col)
    {
        return update(keys, display, tracks->tracks[col - 1]);
    }
};

class App_View_TrackHeader : public App_View_TrackRow {
public:
    App_View_TrackHeader(App_Tracks* _tracks)
        : App_View_TrackRow(_tracks, "      ")
    {
    }

    void renderValue(App_Display_Base* display, uint8_t trackId, App_Audio_Track* track)
    {
        sprintf(display->text + strlen(display->text), "%cTR%d", tracks->trackId == trackId ? '*' : ' ', trackId + 1);
    }
};

class App_View_TrackMasterField : public App_View_TrackRow {
public:
    App_View_TrackMasterField(App_Tracks* _tracks)
        : App_View_TrackRow(_tracks, "VOLUME")
    {
    }

    void renderValue(App_Display_Base* display, uint8_t trackId, App_Audio_Track* track)
    {
        // TBD. to be linked
        strcat(display->text, " ---");
    }
};

class App_View_TrackDelayEnabled : public App_View_TrackRow {
public:
    App_View_TrackDelayEnabled(App_Tracks* _tracks)
        : App_View_TrackRow(_tracks, "DELAY ")
    {
    }

    void renderValue(App_Display_Base* display, uint8_t trackId, App_Audio_Track* track)
    {
        if (track->delayEnabled) {
            strcat(display->text, " ON ");
        } else {
            strcat(display->text, " OFF");
        }
    }

    uint8_t update(UiKeys* keys, App_Display_Base* display, App_Audio_Track* track)
    {
        track->toggleDelay();
        return VIEW_CHANGED;
    }
};

class App_View_Track : public App_View_Table {
protected:
    App_Tracks* tracks;

    App_View_TrackHeader fieldHeader;
    App_View_TrackMasterField masterField;
    App_View_TrackDelayEnabled delayField;

    App_View_TableField* fields[VIEW_TRACK_ROW * VIEW_TRACK_COL] = {
        // clang-format off
        &fieldHeader, &fieldHeader, &fieldHeader, &fieldHeader, &fieldHeader,
        &masterField, &masterField, &masterField, &masterField, &masterField,
        &delayField, &delayField, &delayField, &delayField, &delayField,
        // clang-format on
    };

public:
    App_View_Track(App_Tracks* _tracks)
        : App_View_Table(fields, VIEW_TRACK_ROW, VIEW_TRACK_COL)
        , tracks(_tracks)
        , fieldHeader(_tracks)
        , masterField(_tracks)
        , delayField(_tracks)
    {
        initSelection();
    }

    void initDisplay(App_Display_Base* display)
    {
        display->useColoredHeader();
        display->useColoredLabel();
        App_View_Table::initDisplay(display);
    }

    uint8_t update(UiKeys* keys, App_Display_Base* display)
    {
        int8_t trackId = selectedCol - 1;
        if (trackId != tracks->trackId) {
            tracks->select(trackId);
        }
        return App_View_Table::update(keys, display);
    }
};

#endif