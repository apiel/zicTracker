#ifndef APP_VIEW_INSTRUMENT_H_
#define APP_VIEW_INSTRUMENT_H_

#include "./app_display.h"
#include "./app_tracks.h"
#include "./app_view_table.h"

#define VIEW_INSTR_COL 2
#define VIEW_INSTR_LABELS 6

class App_View_Instrument : public App_View_Table<VIEW_INSTR_LABELS /* 7 */, VIEW_INSTR_COL, VIEW_INSTR_COL> {
protected:
    App_Tracks* tracks;

public:
    App_View_Instrument(App_Tracks* _tracks)
        : App_View_Table(VIEW_INSTR_LABELS)
        , tracks(_tracks)
    {
    }

    void startRow(App_Display* display, uint16_t row) override
    {
        const char label[VIEW_INSTR_LABELS][8] = {
            "Track  ",
            "Instr. ",
            "Type   ",
            "File   ",
            "Env    ",
            "Filter ",
        };

        strcat(display->text, label[row % VIEW_INSTR_LABELS]);
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