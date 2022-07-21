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

public:
    App_View_TrackRow(App_Tracks* _tracks)
        : tracks(_tracks)
    {
    }

    virtual void renderLabel(App_Display* display) = 0;
    virtual void renderValue(App_Display* display, uint8_t trackId, Zic_Seq_Loop_State* state) = 0;

    void render(App_Display* display, uint8_t row, uint8_t col, uint8_t selectedRow, uint8_t selectedCol)
    {
        if (col == 0) {
            renderLabel(display);
        } else {
            uint8_t trackId = col - 1;
            Zic_Seq_Loop_State* state = &tracks->tracks[trackId]->looper.nextState;
            //     if (updatingState && cursor == pos) {
            //         state = &newState;
            //     }
            renderValue(display, trackId, state);
        }
    }
};

class App_View_TrackHeader : public App_View_TrackRow {
public:
    App_View_TrackHeader(App_Tracks* _tracks)
        : App_View_TrackRow(_tracks)
    {
    }

    void renderLabel(App_Display* display)
    {
        strcat(display->text, "   ");
    }

    void renderValue(App_Display* display, uint8_t trackId, Zic_Seq_Loop_State* state)
    {
        sprintf(display->text + strlen(display->text), "%cTR%d", tracks->trackId == trackId ? '*' : ' ', trackId + 1);
    }
};

class App_View_TrackSequence : public App_View_TrackRow {
public:
    App_View_TrackSequence(App_Tracks* _tracks)
        : App_View_TrackRow(_tracks)
    {
    }

    void renderLabel(App_Display* display)
    {
        strcat(display->text, "SEQ");
    }

    void renderValue(App_Display* display, uint8_t trackId, Zic_Seq_Loop_State* state)
    {
        if (state->playing) {
            strcat(display->text, " >ON");
        } else {
            strcat(display->text, " OFF");
        }
    }
};

class App_View_TrackPattern : public App_View_TrackRow {
public:
    App_View_TrackPattern(App_Tracks* _tracks)
        : App_View_TrackRow(_tracks)
    {
    }

    void renderLabel(App_Display* display)
    {
        strcat(display->text, "PAT");
    }

    void renderValue(App_Display* display, uint8_t trackId, Zic_Seq_Loop_State* state)
    {
        sprintf(display->text + strlen(display->text), " %03d", state->pattern->id + 1);
    }
};

class App_View_TrackDetune : public App_View_TrackRow {
public:
    App_View_TrackDetune(App_Tracks* _tracks)
        : App_View_TrackRow(_tracks)
    {
    }

    void renderLabel(App_Display* display)
    {
        strcat(display->text, "DET");
    }

    void renderValue(App_Display* display, uint8_t trackId, Zic_Seq_Loop_State* state)
    {
        sprintf(display->text + strlen(display->text), " %c%02d", state->detune < 0 ? '-' : '+', abs(state->detune));
    }
};

class App_View_TrackMasterField : public App_View_TrackRow {
public:
    App_View_TrackMasterField(App_Tracks* _tracks)
        : App_View_TrackRow(_tracks)
    {
    }

    void renderLabel(App_Display* display)
    {
        strcat(display->text, "-M-");
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
    Zic_Seq_Loop_State newState;
    bool updatingState = false;

    App_View_TrackHeader fieldHeader;
    App_View_TrackSequence sequenceField;
    App_View_TrackPattern patternField;
    App_View_TrackDetune detuneField;
    App_View_TrackMasterField masterField;

    App_View_TableField* fields[VIEW_TRACK_ROW * VIEW_TRACK_COL] = {
        &fieldHeader,
        &fieldHeader,
        &fieldHeader,
        &fieldHeader,
        &fieldHeader,

        &sequenceField,
        &sequenceField,
        &sequenceField,
        &sequenceField,
        &sequenceField,

        &patternField,
        &patternField,
        &patternField,
        &patternField,
        &patternField,

        &detuneField,
        &detuneField,
        &detuneField,
        &detuneField,
        &detuneField,

        &masterField,
        &masterField,
        &masterField,
        &masterField,
        &masterField,
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
    }

    void initDisplay(App_Display* display)
    {
        display->useColoredHeader();
        App_View_Table::initDisplay(display);
    }

    // void renderCell(App_Display* display, uint16_t pos, uint16_t row, uint8_t col)
    // {
    //     if (cursor == pos) {
    //         display->setCursor(3);
    //     }

    //     uint8_t trackId = col;

    //     // TODO ? blink between nextState and state for PAT, SEQ, DET
    //     Zic_Seq_Loop_State* state = &tracks->tracks[trackId]->looper.nextState;
    //     if (updatingState && cursor == pos) {
    //         state = &newState;
    //     }
    //     switch (row) {
    //     case 1:
    //         sprintf(display->text + strlen(display->text), "%03d", state->pattern->id + 1);
    //         break;
    //     case 2:
    //         sprintf(display->text + strlen(display->text),
    //             "%c%02d", state->detune < 0 ? '-' : '+', abs(state->detune));
    //         break;
    //     default:
    //         break;
    //     }
    // }

    uint8_t update(UiKeys* keys, App_Display* display)
    {
        return 0;
        // int8_t row = cursor / VIEW_TRACK_COL;
        // int8_t col = cursor % VIEW_TRACK_COL;
        // int8_t trackId = col;
        // Zic_Seq_Loop_State* nextState = &tracks->looper->nextState;
        // if (keys->A) {
        //     if (!updatingState) {
        //         newState.set(nextState);
        //         updatingState = true;
        //     }
        //     switch (row) {
        //     case 0:
        //         newState.togglePlay();
        //         break;
        //     case 1: {
        //         int8_t direction = 0;
        //         if (keys->Right) {
        //             direction = 1;
        //         } else if (keys->Left) {
        //             direction = -1;
        //         } else if (keys->Up) {
        //             direction = 10;
        //         } else if (keys->Down) {
        //             direction = -10;
        //         }
        //         uint16_t id = newState.pattern->id ? newState.pattern->id : PATTERN_COUNT;
        //         newState.pattern = &tracks->patterns->patterns[(id + direction) % PATTERN_COUNT];
        //         break;
        //     }
        //     case 2:
        //         if (keys->Right) {
        //             newState.set(newState.detune + 1);
        //         } else if (keys->Up) {
        //             newState.set(newState.detune + 12);
        //         } else if (keys->Left) {
        //             newState.set(newState.detune - 1);
        //         } else if (keys->Down) {
        //             newState.set(newState.detune - 12);
        //         }
        //         break;
        //     }
        //     App_View_Table::render(display);
        //     return VIEW_CHANGED;
        // } else if (updatingState) {
        //     // apply newState to nextState only once A is release
        //     nextState->set(&newState);
        //     updatingState = false;
        // }
        // uint8_t res = App_View_Table::update(keys, display);
        // if (trackId != tracks->trackId) {
        //     tracks->select(trackId);
        // }
        // return res;
    }
};

#endif