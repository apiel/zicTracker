#ifndef APP_VIEW_TRACK_H_
#define APP_VIEW_TRACK_H_

#include "./app_display.h"
#include "./app_tracks.h"
#include "./app_view_table.h"

#define VIEW_TRACK_COL 2

class App_View_Track : public App_View_Table<4, VIEW_TRACK_COL, 2> {
protected:
    App_Tracks* tracks;

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

    void renderCell(App_Display* display, uint16_t pos, uint16_t row, uint8_t col)
    {
        // if (tracks->trackId == row) {
        //     display->setCursor(3);
        // }
        if (cursor == pos) {
            display->setCursor(3);
        }

        if (col == 0) {
            if (tracks->tracks[row]->looper.loopOn) {
                strcat(display->text, ">ON");
            } else {
                strcat(display->text, "OFF");
            }
        } else {
            // if tracks->tracks[row]->looper.pattern !=  tracks->tracks[row]->looper.nextPattern
            // we could blink between both ids
            sprintf(display->text + strlen(display->text), "%03d ", tracks->tracks[row]->looper.nextPattern->id + 1);
        }
    }

    uint8_t update(UiKeys* keys, App_Display* display)
    {
        int8_t id = cursor / VIEW_TRACK_COL;
        if (keys->A) {
            if (cursor % VIEW_TRACK_COL == 0) {
                tracks->looper->toggleLoopMode();
            } else if (cursor % VIEW_TRACK_COL == 1) {
                // printf("pattern count %d\n", PATTERN_COUNT);
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
                uint16_t id = tracks->looper->nextPattern->id ? tracks->looper->nextPattern->id : PATTERN_COUNT;
                printf("pat id (%d + %d) %% %d = %d\n",
                    id,
                    direction,
                    PATTERN_COUNT,
                    (id + direction) % PATTERN_COUNT);
                tracks->looper->setNextPattern(
                    &tracks->patterns->patterns[(id + direction) % PATTERN_COUNT]);
            }
            App_View_Table::render(display);
            return VIEW_CHANGED;
        }
        uint8_t res = App_View_Table::update(keys, display);
        if (id != tracks->trackId) {
            tracks->select(id);
        }
        return res;
    }
};

#endif