#ifndef APP_VIEW_TRACK_H_
#define APP_VIEW_TRACK_H_

#include "./app_display.h"
#include "./app_tracks.h"
#include "./app_view_table.h"

#include <zic_seq_pattern.h>

#define VIEW_TRACK_ROW 5
#define VIEW_TRACK_COL 5

class App_View_TrackLabel : public App_View_TableField {
protected:
    const char* labels[VIEW_TRACK_ROW] = {
        "   ",
        "SEQ",
        "PAT",
        "DET",
        "-M-"
    };

public:
    void render(App_Display* display, uint8_t row, uint8_t col, uint8_t selectedRow, uint8_t selectedCol)
    {
        strcat(display->text, labels[row]);
    }
};

class App_View_TrackHeader : public App_View_TableField {
protected:
    App_Tracks* tracks;

public:
    App_View_TrackHeader(App_Tracks* _tracks)
        : tracks(_tracks)
    {
    }
    void render(App_Display* display, uint8_t row, uint8_t col, uint8_t selectedRow, uint8_t selectedCol)
    {
        sprintf(display->text + strlen(display->text), "%cTR%d", tracks->trackId == col - 1 ? '*' : ' ', col);
    }
};

// class App_View_TrackSequenceLabel : public App_View_TableField {
// public:
//     void render(App_Display* display, uint8_t row, uint8_t col, uint8_t selectedRow, uint8_t selectedCol)
//     {
//         strcat(display->text, "SEQ ");
//     }
// };

// class App_View_TrackPatternLabel : public App_View_TableField {
// public:
//     void render(App_Display* display, uint8_t row, uint8_t col, uint8_t selectedRow, uint8_t selectedCol)
//     {
//         strcat(display->text, "PAT ");
//     }
// };

// class App_View_TrackDetuneLabel : public App_View_TableField {
// public:
//     void render(App_Display* display, uint8_t row, uint8_t col, uint8_t selectedRow, uint8_t selectedCol)
//     {
//         strcat(display->text, "DET ");
//     }
// };

// class App_View_TrackMasterLabel : public App_View_TableField {
// public:
//     void render(App_Display* display, uint8_t row, uint8_t col, uint8_t selectedRow, uint8_t selectedCol)
//     {
//         strcat(display->text, "-M- ");
//     }
// };

class App_View_Track : public App_View_Table {
protected:
    App_Tracks* tracks;
    Zic_Seq_Loop_State newState;
    bool updatingState = false;

    App_View_TrackLabel fieldLabel;
    App_View_TrackHeader fieldHeader;
    // App_View_TrackPatternLabel patternFieldLabel;
    // App_View_TrackDetuneLabel detuneFieldLabel;
    // App_View_TrackMasterLabel masterFieldLabel;

    App_View_TableField* fields[VIEW_TRACK_ROW * VIEW_TRACK_COL] = {
        &fieldLabel,
        &fieldHeader,
        &fieldHeader,
        &fieldHeader,
        &fieldHeader,

        &fieldLabel,
        NULL,
        NULL,
        NULL,
        NULL,

        &fieldLabel,
        NULL,
        NULL,
        NULL,
        NULL,

        &fieldLabel,
        NULL,
        NULL,
        NULL,
        NULL,

        &fieldLabel,
        NULL,
        NULL,
        NULL,
        NULL,
    };

public:
    App_View_Track(App_Tracks* _tracks)
        : App_View_Table(fields, VIEW_TRACK_ROW, VIEW_TRACK_COL)
        , tracks(_tracks)
        , fieldHeader(_tracks)
    {
    }

    void initDisplay(App_Display* display)
    {
        display->useColoredHeader();
        App_View_Table::initDisplay(display);
    }

    // void startRow(App_Display* display, uint16_t row)
    // {
    //     display->useColoredLabel();
    //     switch (row) {
    //     case 0:
    //         strcat(display->text, "SEQ ");
    //         break;

    //     case 1:
    //         strcat(display->text, "PAT ");
    //         break;

    //     case 2:
    //         strcat(display->text, "DET ");
    //         break;

    //     case 3:
    //         // master linked
    //         strcat(display->text, "-M- ");
    //         break;

    //     default:
    //         break;
    //     }
    // }

    // void initDisplay(App_Display* display)
    // {
    //     display->useColoredHeader();
    //     sprintf(display->text, "   %cTR1%cTR2%cTR3%cTR4\n",
    //         tracks->trackId == 0 ? '*' : ' ',
    //         tracks->trackId == 1 ? '*' : ' ',
    //         tracks->trackId == 2 ? '*' : ' ',
    //         tracks->trackId == 3 ? '*' : ' '); //     SEQ PAT DET M
    // }

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
    //     case 0:
    //         if (state->playing) {
    //             strcat(display->text, ">ON");
    //         } else {
    //             strcat(display->text, "OFF");
    //         }
    //         break;
    //     case 1:
    //         sprintf(display->text + strlen(display->text), "%03d", state->pattern->id + 1);
    //         break;
    //     case 2:
    //         sprintf(display->text + strlen(display->text),
    //             "%c%02d", state->detune < 0 ? '-' : '+', abs(state->detune));
    //         break;
    //     case 3:
    //         // master linked
    //         strcat(display->text, "---");
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