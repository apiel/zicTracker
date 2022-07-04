#ifndef APP_VIEW_INSTRUMENT_H_
#define APP_VIEW_INSTRUMENT_H_

#include "./app_display.h"
#include "./app_tracks.h"
#include "./app_view_table.h"

#define VIEW_TRACK_COL 2

class App_View_Instrument : public App_View_Table<5, VIEW_TRACK_COL, VIEW_TRACK_COL> {
protected:
    App_Tracks* tracks;

public:
    App_View_Instrument(App_Tracks* _tracks)
        : App_View_Table(4)
        , tracks(_tracks)
    {
    }

    void startRow(App_Display* display, uint16_t row) override
    {
        switch (row) {
        case 0:
            strcat(display->text, "Track  ");
            break;

        case 1:
            strcat(display->text, "Instr. ");
            break;

        case 2:
            strcat(display->text, "File   ");
            break;

        case 3:
            strcat(display->text, "Env    ");
            break;

        case 4:
            strcat(display->text, "Filter ");
            break;

        default:
            break;
        }
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
            sprintf(display->text + strlen(display->text), "%03d ", 1);
        }
    }

    uint8_t update(UiKeys* keys, App_Display* display)
    {
        uint8_t res = App_View_Table::update(keys, display);

        return res;
    }
};

#endif