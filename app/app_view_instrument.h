#ifndef APP_VIEW_INSTRUMENT_H_
#define APP_VIEW_INSTRUMENT_H_

#include "./app_display.h"
#include "./app_tracks.h"
#include "./app_view_table.h"

#define VIEW_INSTR_COL 2
#define VIEW_INSTR_LABELS 7

class App_View_Instrument : public App_View_Table<7, VIEW_INSTR_COL, VIEW_INSTR_COL> {
protected:
    App_Tracks* tracks;

    uint8_t cursorSizeTable[VIEW_INSTR_LABELS * VIEW_INSTR_COL] = {
        3, 0,
        3, 0,
        9, 0,
        12, 0,
        4, 0,
        3, 3,
        3, 3
    };

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
            "Wav    ",
            "Level  ",
            "Env    ",
            "Filter ",
        };

        strcat(display->text, label[row % VIEW_INSTR_LABELS]);
    }

    void renderCell(App_Display* display, uint16_t pos, uint16_t row, uint8_t col)
    {
        display->useColoredLabel();
        if (cursor == pos) {
            if (cursorSizeTable[pos]) {
                display->setCursor(cursorSizeTable[pos]);
            } else {
                // If current cursor is 0 we get the cursor from the previous field
                display->setCursor(cursorSizeTable[pos - 1], -(cursorSizeTable[pos - 1] + 1));
            }
        }

        switch (row) {
        case 0:
            if (col == 0) {
                strcat(display->text, "1  ");
            }
            break;

        case 1:
            if (col == 0) {
                strcat(display->text, "A  ");
            }
            break;

        case 2:
            if (col == 0) {
                strcat(display->text, "Wavetable");
            }
            break;

        case 3:
            if (col == 0) {
                strcat(display->text, "sine        ");
            }
            break;

        case 4:
            if (col == 0) {
                strcat(display->text, "100%");
            }
            break;

        case 5:
            if (col == 0) {
                strcat(display->text, "50 ");
            } else if (col == 1) {
                strcat(display->text, "150");
            }
            break;

        case 6:
            if (col == 0) {
                strcat(display->text, "127");
            } else if (col == 1) {
                strcat(display->text, "0  ");
            }
            break;

        default:
            break;
        }
    }

    uint8_t update(UiKeys* keys, App_Display* display)
    {
        uint8_t res = App_View_Table::update(keys, display);

        return res;
    }
};

#endif