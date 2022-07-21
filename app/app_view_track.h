#ifndef APP_VIEW_TRACK_H_
#define APP_VIEW_TRACK_H_

#include "./app_display.h"
#include "./app_tracks.h"
#include "./app_view_table.h"

#include <zic_seq_pattern.h>

#define VIEW_TRACK_ROW 5
#define VIEW_TRACK_COL 5

class App_View_TrackRow : public App_View_TableField {
protected:
    App_Tracks* tracks;
    Zic_Seq_Loop_State newState;
    bool updating = false;
    const char * label;

public:
    App_View_TrackRow(App_Tracks* _tracks, const char * _label)
        : tracks(_tracks)
        , label(_label)
    {
    }

    virtual void renderValue(App_Display* display, uint8_t trackId, Zic_Seq_Loop_State* state) = 0;

    bool isSelectable(uint8_t row, uint8_t col) override
    {
        return col != 0 && row != 0;
    }

    void render(App_Display* display, uint8_t row, uint8_t col, uint8_t selectedRow, uint8_t selectedCol)
    {
        if (col == 0) {
            strcat(display->text, label);
        } else {
            if (selectedRow == row && selectedCol == col) {
                display->setCursor(3, 1);
            }
            uint8_t trackId = col - 1;
            // printf("%d col %d row %d updating %d %d %d\n", trackId, col, row, updating, col == selectedCol, row == selectedRow);
            Zic_Seq_Loop_State* state = updating && col == selectedCol && row == selectedRow ? &newState : &tracks->tracks[trackId]->looper.nextState;

            renderValue(display, trackId, state);
        }
    }

    void updateStart()
    {
        Zic_Seq_Loop_State* nextState = &tracks->looper->nextState;
        newState.set(nextState);
        updating = true;
    }

    void updateEnd()
    {
        Zic_Seq_Loop_State* nextState = &tracks->looper->nextState;
        nextState->set(&newState);
        updating = false;
    }
};

class App_View_TrackHeader : public App_View_TrackRow {
public:
    App_View_TrackHeader(App_Tracks* _tracks)
        : App_View_TrackRow(_tracks, "   ")
    {
    }

    void renderValue(App_Display* display, uint8_t trackId, Zic_Seq_Loop_State* state)
    {
        sprintf(display->text + strlen(display->text), "%cTR%d", tracks->trackId == trackId ? '*' : ' ', trackId + 1);
    }
};

class App_View_TrackSequence : public App_View_TrackRow {
public:
    App_View_TrackSequence(App_Tracks* _tracks)
        : App_View_TrackRow(_tracks, "SEQ")
    {
    }

    void renderValue(App_Display* display, uint8_t trackId, Zic_Seq_Loop_State* state)
    {
        if (state->playing) {
            strcat(display->text, " >ON");
        } else {
            strcat(display->text, " OFF");
        }
    }

    uint8_t update(UiKeys* keys, App_Display* display)
    {
        newState.togglePlay();
        return VIEW_CHANGED;
    }
};

class App_View_TrackPattern : public App_View_TrackRow {
public:
    App_View_TrackPattern(App_Tracks* _tracks)
        : App_View_TrackRow(_tracks, "PAT")
    {
    }

    void renderValue(App_Display* display, uint8_t trackId, Zic_Seq_Loop_State* state)
    {
        sprintf(display->text + strlen(display->text), " %03d", state->pattern->id + 1);
    }

    uint8_t update(UiKeys* keys, App_Display* display)
    {
        int8_t direction = 0;
        if (keys->Right) {
            direction = 1;
        } else if (keys->Left) {
            direction = -1;
        } else if (keys->Up) {
            direction = 10;
        } else if (keys->Down) {
            direction = -10;
        }
        uint16_t id = newState.pattern->id ? newState.pattern->id : PATTERN_COUNT;
        newState.pattern = &tracks->patterns->patterns[(id + direction) % PATTERN_COUNT];
        return VIEW_CHANGED;
    }
};

class App_View_TrackDetune : public App_View_TrackRow {
public:
    App_View_TrackDetune(App_Tracks* _tracks)
        : App_View_TrackRow(_tracks, "DET")
    {
    }

    void renderValue(App_Display* display, uint8_t trackId, Zic_Seq_Loop_State* state)
    {
        sprintf(display->text + strlen(display->text), " %c%02d", state->detune < 0 ? '-' : '+', abs(state->detune));
    }

    uint8_t update(UiKeys* keys, App_Display* display)
    {
        if (keys->Right) {
            newState.set(newState.detune + 1);
        } else if (keys->Up) {
            newState.set(newState.detune + 12);
        } else if (keys->Left) {
            newState.set(newState.detune - 1);
        } else if (keys->Down) {
            newState.set(newState.detune - 12);
        }
        return VIEW_CHANGED;
    }
};

class App_View_TrackMasterField : public App_View_TrackRow {
public:
    App_View_TrackMasterField(App_Tracks* _tracks)
        : App_View_TrackRow(_tracks, "-M-")
    {
    }

    void renderValue(App_Display* display, uint8_t trackId, Zic_Seq_Loop_State* state)
    {
        // TBD. to be linked
        strcat(display->text, " ---");
    }
};

class App_View_Track : public App_View_Table {
protected:
    App_Tracks* tracks;

    App_View_TrackHeader fieldHeader;
    App_View_TrackSequence sequenceField;
    App_View_TrackPattern patternField;
    App_View_TrackDetune detuneField;
    App_View_TrackMasterField masterField;

    App_View_TableField* fields[VIEW_TRACK_ROW * VIEW_TRACK_COL] = {
        // clang-format off
        &fieldHeader, &fieldHeader, &fieldHeader, &fieldHeader, &fieldHeader,
        &sequenceField, &sequenceField, &sequenceField, &sequenceField, &sequenceField,
        &patternField, &patternField, &patternField, &patternField, &patternField,
        &detuneField, &detuneField, &detuneField, &detuneField, &detuneField,
        &masterField, &masterField, &masterField, &masterField, &masterField,
        // clang-format on
    };

public:
    App_View_Track(App_Tracks* _tracks)
        : App_View_Table(fields, VIEW_TRACK_ROW, VIEW_TRACK_COL)
        , tracks(_tracks)
        , fieldHeader(_tracks)
        , sequenceField(_tracks)
        , patternField(_tracks)
        , detuneField(_tracks)
        , masterField(_tracks)
    {
        initSelection();
    }

    void initDisplay(App_Display* display)
    {
        display->useColoredHeader();
        display->useColoredLabel();
        App_View_Table::initDisplay(display);
    }

    uint8_t update(UiKeys* keys, App_Display* display)
    {
        int8_t trackId = selectedCol - 1;
        if (trackId != tracks->trackId) {
            tracks->select(trackId);
        }
        return App_View_Table::update(keys, display);
    }
};

#endif