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
        2, 0,
        2, 0,
        12, 0,
        12, 0,
        5, 0,
        5, 5,
        5, 5
    };

    uint8_t instrument = 0;

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
        App_Instrument* synth = tracks->track->synths[instrument];
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
                sprintf(display->text + strlen(display->text), "%-2d", tracks->trackId + 1);
            }
            break;

        case 1:
            if (col == 0) {
                sprintf(display->text + strlen(display->text), "%-2c", instrument + 'A');
            }
            break;

        case 2:
            if (col == 0) {
                if (synth->isWavetable) {
                    strcat(display->text, "Wavetable   ");
                } else {
                    strcat(display->text, "Sample      ");
                }
            }
            break;

        case 3:
            if (col == 0) {
                char filename[12];
                strncpy(filename, synth->filename, 12);
                sprintf(display->text + strlen(display->text), "%-12s", filename);
            }
            break;

        case 4:
            if (col == 0) {
                strcat(display->text, "100 %");
            }
            break;

        case 5:
            if (col == 0) {
                // strcat(display->text, "50 ");
                sprintf(display->text + strlen(display->text), "%-5d", synth->asr.getAttack());
            } else if (col == 1) {
                // strcat(display->text, "150");
                sprintf(display->text + strlen(display->text), "%-5d", synth->asr.getRelease());
            }
            break;

        case 6:
            if (col == 0) {
                strcat(display->text, "127  ");
            } else if (col == 1) {
                strcat(display->text, "0    ");
            }
            break;

        default:
            break;
        }
    }

    uint8_t update(UiKeys* keys, App_Display* display)
    {
        App_Instrument* synth = tracks->track->synths[instrument];

        int8_t row = cursor / VIEW_TRACK_COL;
        if (keys->A) {
            if (cursor % VIEW_TRACK_COL == 0) {
                switch (row) {
                case 0:
                    if (keys->Right || keys->Up) {
                        tracks->select(tracks->trackId + 1);
                    } else if (keys->Left || keys->Down) {
                        tracks->select(tracks->trackId - 1);
                    }
                    break;

                case 1:
                    if (keys->Right || keys->Up) {
                        if (instrument + 1 < INSTRUMENT_COUNT) {
                            instrument++;
                        }
                    } else if (keys->Left || keys->Down) {
                        instrument = instrument ? instrument - 1 % INSTRUMENT_COUNT : 0;
                    }
                    break;

                case 2:
                    if (keys->Right || keys->Up) {
                        synth->isWavetable = true;
                    } else if (keys->Left || keys->Down) {
                        synth->isWavetable = false;
                    }
                    synth->setNext(0);
                    break;

                case 3:
                    if (keys->Right || keys->Up) {
                        synth->setNext(+1);
                    } else if (keys->Left || keys->Down) {
                        synth->setNext(-1);
                    }
                    break;

                default:
                    break;
                }
            } else if (cursor % VIEW_TRACK_COL == 1) {
                // int8_t direction = 0;
                // if (keys->Right) {
                //     direction = 1;
                // } else if (keys->Left) {
                //     direction = -1;
                // } else if (keys->Up) {
                //     direction = 10;
                // } else if (keys->Down) {
                //     direction = -10;
                // }
            }
            App_View_Table::render(display);
            return VIEW_CHANGED;
        }

        uint8_t res = App_View_Table::update(keys, display);

        return res;
    }
};

#endif