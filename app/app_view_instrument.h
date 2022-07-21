#ifndef APP_VIEW_INSTRUMENT_H_
#define APP_VIEW_INSTRUMENT_H_

#include "./app_display.h"
#include "./app_tracks.h"
#include "./app_view_table.h"

#define VIEW_INSTR_ROW 7
#define VIEW_INSTR_COL 3
#define VIEW_INSTR_LABELS 7

class App_View_InstrumentRow : public App_View_TableField {
protected:
    App_Tracks* tracks;
    uint8_t cursorLen;
    uint8_t instrument = 0;

public:
    App_View_InstrumentRow(App_Tracks* _tracks, uint8_t _cursorLen = 5)
        : tracks(_tracks)
        , cursorLen(_cursorLen)
    {
    }

    virtual void renderLabel(App_Display* display) = 0;
    virtual void renderValue(App_Display* display, App_Instrument* synth) = 0;

    bool isSelectable(uint8_t row, uint8_t col) override
    {
        return col != 0;
    }

    void render(App_Display* display, uint8_t row, uint8_t col, uint8_t selectedRow, uint8_t selectedCol)
    {
        if (col == 0) {
            renderLabel(display);
        } else {
            if (selectedRow == row && selectedCol == col) {
                display->setCursor(cursorLen);
            }

            App_Instrument* synth = tracks->track->synths[instrument];
            renderValue(display, synth);
        }
    }
};

class App_View_InstrumentTrack : public App_View_InstrumentRow {
public:
    App_View_InstrumentTrack(App_Tracks* _tracks)
        : App_View_InstrumentRow(_tracks, 2)
    {
    }

    void renderLabel(App_Display* display)
    {
        strcat(display->text, "Track  ");
    }

    void renderValue(App_Display* display, App_Instrument* synth)
    {
        sprintf(display->text + strlen(display->text), "%-2d", tracks->trackId + 1);
    }

    uint8_t update(UiKeys* keys, App_Display* display) override
    {
        if (keys->Right || keys->Up) {
            tracks->select(tracks->trackId + 1);
        } else if (keys->Left || keys->Down) {
            tracks->select(tracks->trackId - 1);
        }
        return VIEW_CHANGED;
    }
};

class App_View_InstrumentInstr : public App_View_InstrumentRow {
public:
    App_View_InstrumentInstr(App_Tracks* _tracks)
        : App_View_InstrumentRow(_tracks, 2)
    {
    }

    void renderLabel(App_Display* display)
    {
        strcat(display->text, "Instr. ");
    }

    void renderValue(App_Display* display, App_Instrument* synth)
    {
        sprintf(display->text + strlen(display->text), "%-2c", instrument + 'A');
    }

    uint8_t update(UiKeys* keys, App_Display* display) override
    {
        // TODO need to male instrument pointer
        if (keys->Right || keys->Up) {
            if (instrument + 1 < INSTRUMENT_COUNT) {
                instrument++;
            }
        } else if (keys->Left || keys->Down) {
            instrument = instrument ? instrument - 1 % INSTRUMENT_COUNT : 0;
        }
        return VIEW_CHANGED;
    }
};

class App_View_InstrumentType : public App_View_InstrumentRow {
public:
    App_View_InstrumentType(App_Tracks* _tracks)
        : App_View_InstrumentRow(_tracks, 12)
    {
    }

    void renderLabel(App_Display* display)
    {
        strcat(display->text, "Type   ");
    }

    void renderValue(App_Display* display, App_Instrument* synth)
    {
        if (synth->isWavetable) {
            strcat(display->text, "Wavetable   ");
        } else {
            strcat(display->text, "Sample      ");
        }
    }

    uint8_t update(UiKeys* keys, App_Display* display) override
    {
        // FIXME
        // if (keys->Right || keys->Up) {
        //     synth->isWavetable = true;
        // } else if (keys->Left || keys->Down) {
        //     synth->isWavetable = false;
        // }
        // synth->setNext();
        return VIEW_CHANGED;
    }
};

class App_View_Instrument : public App_View_Table {
protected:
    App_Tracks* tracks;

    uint8_t cursorSizeTable[VIEW_INSTR_LABELS * VIEW_INSTR_COL] = {
        12, 0,
        5, 5,
        5, 5,
        5, 5
    };

    uint8_t instrument = 0;

    App_View_InstrumentTrack trackField;
    App_View_InstrumentInstr instrField;
    App_View_InstrumentType typeField;
    App_View_TableField* fields[VIEW_INSTR_ROW * VIEW_INSTR_COL] = {
        // clang-format off
        &trackField, &trackField, NULL,
        &instrField, &instrField, NULL,
        &typeField, &typeField, NULL,
        // clang-format on
    };

public:
    App_View_Instrument(App_Tracks* _tracks)
        : App_View_Table(fields, VIEW_INSTR_ROW, VIEW_INSTR_COL)
        , tracks(_tracks)
        , trackField(_tracks)
        , instrField(_tracks)
        , typeField(_tracks)
    {
        initSelection();
    }

    void initDisplay(App_Display* display)
    {
        display->useColoredLabel();
        App_View_Table::initDisplay(display);
    }

    // void startRow(App_Display* display, uint16_t row) override
    // {
    //     const char label[VIEW_INSTR_LABELS][8] = {
    //         ,
    //         "Wav    ",
    //         "Level  ",
    //         "Env    ",
    //         "Filter ",
    //     };

    //     strcat(display->text, label[row % VIEW_INSTR_LABELS]);
    // }

    // void renderCell(App_Display* display, uint16_t pos, uint16_t row, uint8_t col)
    // {
    //     case 2:
    //         if (col == 0) {

    //         }
    //         break;

    //     case 3:
    //         if (col == 0) {
    //             char filename[12];
    //             strncpy(filename, synth->filename, 12);
    //             sprintf(display->text + strlen(display->text), "%-12s", filename);
    //         }
    //         break;

    //     case 4:
    //         if (col == 0) {
    //             sprintf(display->text + strlen(display->text), "%-3d%% ", synth->wave.getAmplitude());
    //         } else {
    //             if (synth->wave.isMuted()) {
    //                 strcat(display->text, "Muted");
    //             } else {
    //                 strcat(display->text, ">ON  ");
    //             }
    //         }
    //         break;

    //     case 5:
    //         if (col == 0) {
    //             sprintf(display->text + strlen(display->text), "%-5d", synth->asr.getAttack());
    //         } else if (col == 1) {
    //             sprintf(display->text + strlen(display->text), "%-5d", synth->asr.getRelease());
    //         }
    //         break;

    //     case 6:
    //         // could remove leading 0
    //         if (col == 0) {
    //             sprintf(display->text + strlen(display->text), "%.3f", synth->filter.cutoff);
    //         } else if (col == 1) {
    //             sprintf(display->text + strlen(display->text), "%.3f", synth->filter.resonance);
    //         }
    //         break;

    //     default:
    //         break;
    //     }
    // }

    uint8_t update(UiKeys* keys, App_Display* display)
    {
        return App_View_Table::update(keys, display);
        // App_Instrument* synth = tracks->track->synths[instrument];

        // int8_t row = cursor / VIEW_INSTR_COL;
        // int8_t col = cursor % VIEW_INSTR_COL;
        // printf("row: %d, col: %d\n", row, col);
        // if (keys->A) {
        //     switch (row) {

        //     case 1:
        //         break;

        //     case 2:
        //         break;

        //     case 3:
        //         if (keys->Right || keys->Up) {
        //             synth->setNext(+1);
        //         } else if (keys->Left || keys->Down) {
        //             synth->setNext(-1);
        //         }
        //         break;

        //     case 4:
        //         if (col == 0) {
        //             if (keys->Right) {
        //                 synth->wave.setAmplitude(synth->wave.getAmplitude() + 1);
        //             } else if (keys->Up) {
        //                 synth->wave.setAmplitude(synth->wave.getAmplitude() + 10);
        //             } else if (keys->Left) {
        //                 synth->wave.setAmplitude(synth->wave.getAmplitude() - 1);
        //             } else if (keys->Down) {
        //                 synth->wave.setAmplitude(synth->wave.getAmplitude() - 10);
        //             }
        //         } else {
        //             if (synth->wave.isMuted()) {
        //                 synth->wave.setMute(false);
        //             } else {
        //                 synth->wave.setMute();
        //             }
        //         }
        //         break;

        //     case 5:
        //         if (col == 0) {
        //             if (keys->Right) {
        //                 synth->asr.setAttack(synth->asr.getAttack() + 1);
        //             } else if (keys->Up) {
        //                 synth->asr.setAttack(synth->asr.getAttack() + 10);
        //             } else if (keys->Left) {
        //                 synth->asr.setAttack(synth->asr.getAttack() - 1);
        //             } else if (keys->Down) {
        //                 synth->asr.setAttack(synth->asr.getAttack() - 10);
        //             }
        //         } else {
        //             if (keys->Right) {
        //                 synth->asr.setRelease(synth->asr.getRelease() + 1);
        //             } else if (keys->Up) {
        //                 synth->asr.setRelease(synth->asr.getRelease() + 10);
        //             } else if (keys->Left) {
        //                 synth->asr.setRelease(synth->asr.getRelease() - 1);
        //             } else if (keys->Down) {
        //                 synth->asr.setRelease(synth->asr.getRelease() - 10);
        //             }
        //         }
        //         break;

        //     case 6:
        //         if (col == 0) {
        //             if (keys->Right) {
        //                 synth->filter.setCutoff(synth->filter.cutoff + 0.001);
        //             } else if (keys->Up) {
        //                 synth->filter.setCutoff(synth->filter.cutoff + 0.010);
        //             } else if (keys->Left) {
        //                 synth->filter.setCutoff(synth->filter.cutoff - 0.001);
        //             } else if (keys->Down) {
        //                 synth->filter.setCutoff(synth->filter.cutoff - 0.010);
        //             }
        //         } else {
        //             if (keys->Right) {
        //                 synth->filter.setResonance(synth->filter.resonance + 0.001);
        //             } else if (keys->Up) {
        //                 synth->filter.setResonance(synth->filter.resonance + 0.010);
        //             } else if (keys->Left) {
        //                 synth->filter.setResonance(synth->filter.resonance - 0.001);
        //             } else if (keys->Down) {
        //                 synth->filter.setResonance(synth->filter.resonance - 0.010);
        //             }
        //         }
        //         break;

        //     default:
        //         break;
        //     }
        //     App_View_Table::render(display);
        //     return VIEW_CHANGED;
        // }

        // uint8_t res = App_View_Table::update(keys, display);

        // return res;
    }
};

#endif