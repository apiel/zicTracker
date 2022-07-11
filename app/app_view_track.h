#ifndef APP_VIEW_TRACK_H_
#define APP_VIEW_TRACK_H_

#include "./app_display.h"
#include "./app_tracks.h"
#include "./app_view_table.h"

#include <zic_seq_pattern.h>

#define VIEW_TRACK_COL 2

class App_View_Track : public App_View_Table<4, VIEW_TRACK_COL, 2> {
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
        sprintf(display->text + strlen(display->text), "%cTrack%d ", tracks->trackId == row ? '*' : ' ', row + 1);
    }

    void initDisplay(App_Display* display) override
    {
        display->useColoredHeader();
        strcpy(display->text, "        SEQ PAT\n");
    }

    void renderCell(App_Display* display, uint16_t pos, uint16_t row, uint8_t col)
    {
        display->useColoredLabel(1);
        if (cursor == pos) {
            display->setCursor(3);
        }

        uint8_t trackId = row;
        if (col == 0) {
            if (tracks->tracks[trackId]->looper.loopOn) {
                strcat(display->text, ">ON");
            } else {
                strcat(display->text, "OFF");
            }
        } else {
            // if tracks->tracks[row]->looper.pattern !=  tracks->tracks[row]->looper.nextPattern
            // we could blink between both ids
            sprintf(display->text + strlen(display->text), "%03d ",
                (nextPat != NULL && cursor == pos ? nextPat->id : tracks->tracks[trackId]->looper.nextPattern->id) + 1);
        }

        // TODO
        // add more column:
        // - detune value
        // - master link
        // change behavior:
        // - on/off should start/stop the loop without the need to click on keyboard
        // (however, pressing detune would still trigger the loop even if it off)
    }

    uint8_t update(UiKeys* keys, App_Display* display)
    {
        int8_t row = cursor / VIEW_TRACK_COL;
        int8_t trackId = row;
        if (keys->A) {
            if (cursor % VIEW_TRACK_COL == 0) {
                tracks->looper->toggleLoopMode();
            } else if (cursor % VIEW_TRACK_COL == 1) {
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
                    nextPat = tracks->tracks[trackId]->looper.nextPattern;
                }
                uint16_t id = nextPat->id ? nextPat->id : PATTERN_COUNT;
                nextPat = &tracks->patterns->patterns[(id + direction) % PATTERN_COUNT];
            }
            App_View_Table::render(display);
            return VIEW_CHANGED;
        } else if (nextPat) {
            // apply next pattern only once A is release
            tracks->looper->setNextPattern(nextPat);
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