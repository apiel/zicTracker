#ifndef APP_VIEW_TRACK_H_
#define APP_VIEW_TRACK_H_

#include "./app_display.h"
#include "./app_tracks.h"
#include "./app_view_table.h"

#include <zic_seq_pattern.h>

#define VIEW_TRACK_COL 4

class App_View_Track : public App_View_Table<4, VIEW_TRACK_COL, VIEW_TRACK_COL> {
protected:
    App_Tracks* tracks;
    Zic_Seq_Pattern* nextPat = NULL;

public:
    App_View_Track(App_Tracks* _tracks)
        : App_View_Table(4)
        , tracks(_tracks)
    {
    }

    void startRow(App_Display* display, uint16_t row) override
    {
        display->useColoredLabel();
        switch (row) {
        case 0:
            strcat(display->text, "SEQ ");
            break;

        case 1:
            strcat(display->text, "PAT ");
            break;

        case 2:
            strcat(display->text, "DET ");
            break;

        case 3:
            // master linked
            strcat(display->text, "-M- ");
            break;

        default:
            break;
        }
    }

    void initDisplay(App_Display* display) override
    {
        display->useColoredHeader();
        sprintf(display->text, "   %cTR1%cTR2%cTR3%cTR4\n",
            tracks->trackId == 0 ? '*' : ' ',
            tracks->trackId == 1 ? '*' : ' ',
            tracks->trackId == 2 ? '*' : ' ',
            tracks->trackId == 3 ? '*' : ' '); //     SEQ PAT DET M
    }

    void renderCell(App_Display* display, uint16_t pos, uint16_t row, uint8_t col)
    {
        if (cursor == pos) {
            display->setCursor(3);
        }

        uint8_t trackId = col;

        // TODO ? blink between nextState and state
        Zic_Seq_Loop_State* state = &tracks->tracks[trackId]->looper.nextState;
        switch (row) {
        case 0:
            // TODO blink bettwen on off when it will stop at the end of the pattern
            if (state->playing) {
                strcat(display->text, ">ON");
            } else {
                strcat(display->text, "OFF");
            }
            break;
        case 1:
            // if tracks->tracks[row]->looper.pattern !=  tracks->tracks[row]->looper.nextPattern
            // we could blink between both ids
            sprintf(display->text + strlen(display->text), "%03d",
                (nextPat != NULL && cursor == pos ? nextPat->id : state->pattern->id) + 1);
            break;
        case 2:
            sprintf(display->text + strlen(display->text),
                "%c%02d", state->detune < 0 ? '-' : '+', abs(state->detune));
            break;
        case 3:
            // master linked
            strcat(display->text, "---");
            break;
        default:
            break;
        }
    }

    uint8_t update(UiKeys* keys, App_Display* display)
    {
        int8_t row = cursor / VIEW_TRACK_COL;
        int8_t col = cursor % VIEW_TRACK_COL;
        int8_t trackId = col;
        Zic_Seq_Loop_State* nextState = &tracks->looper->nextState;
        if (keys->A) {
            switch (row) {
            case 0:
                nextState->togglePlay();
                break;
            case 1: {
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
                if (nextPat == NULL) {
                    nextPat = nextState->pattern;
                }
                uint16_t id = nextPat->id ? nextPat->id : PATTERN_COUNT;
                nextPat = &tracks->patterns->patterns[(id + direction) % PATTERN_COUNT];
                break;
            }
            case 2:
                if (keys->Right) {
                    nextState->set(nextState->detune + 1);
                } else if (keys->Up) {
                    nextState->set(nextState->detune + 12);
                } else if (keys->Left) {
                    nextState->set(nextState->detune - 1);
                } else if (keys->Down) {
                    nextState->set(nextState->detune - 12);
                }
                break;
            }
            App_View_Table::render(display);
            return VIEW_CHANGED;
        } else if (nextPat) {
            // apply next pattern only once A is release
            nextState->setPattern(nextPat);
            nextPat = NULL;
        }
        uint8_t res = App_View_Table::update(keys, display);
        if (trackId != tracks->trackId) {
            tracks->select(trackId);
        }
        return res;
    }
};

#endif