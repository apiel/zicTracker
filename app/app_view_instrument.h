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
    const char* label;
    App_Tracks* tracks;
    uint8_t cursorLen;
    uint8_t* instrument;

public:
    App_View_InstrumentRow(App_Tracks* _tracks, uint8_t* _instrument, const char* _label, uint8_t _cursorLen = 5)
        : label(_label)
        , tracks(_tracks)
        , cursorLen(_cursorLen)
        , instrument(_instrument)
    {
    }

    virtual void renderValue(App_Display* display, uint8_t col) = 0;

    bool isSelectable(uint8_t row, uint8_t col) override
    {
        return col != 0;
    }

    App_Instrument* getSynth()
    {
        return tracks->track->synths[*instrument];
    }

    void render(App_Display* display, uint8_t row, uint8_t col, uint8_t selectedRow, uint8_t selectedCol)
    {
        if (col == 0) {
            strcat(display->text, label);
        } else {
            if (selectedRow == row && selectedCol == col) {
                display->setCursor(cursorLen);
            }

            renderValue(display, col);
        }
    }
};

class App_View_InstrumentTrack : public App_View_InstrumentRow {
public:
    App_View_InstrumentTrack(App_Tracks* _tracks, uint8_t* _instrument)
        : App_View_InstrumentRow(_tracks, _instrument, "Track  ", 2)
    {
    }

    void renderValue(App_Display* display, uint8_t col)
    {
        sprintf(display->text + strlen(display->text), "%-2d", tracks->trackId + 1);
    }

    uint8_t update(UiKeys* keys, App_Display* display, uint8_t row, uint8_t col) override
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
    App_View_InstrumentInstr(App_Tracks* _tracks, uint8_t* _instrument)
        : App_View_InstrumentRow(_tracks, _instrument, "Instr. ", 2)
    {
    }

    void renderValue(App_Display* display, uint8_t col)
    {
        sprintf(display->text + strlen(display->text), "%-2c", *instrument + 'A');
    }

    uint8_t update(UiKeys* keys, App_Display* display, uint8_t row, uint8_t col) override
    {
        uint8_t direction = 0;
        if (keys->Right || keys->Up) {
            direction = +1;
        } else if (keys->Left || keys->Down) {
            direction = -1;
        }
        *instrument = (*instrument + INSTRUMENT_COUNT + direction) % INSTRUMENT_COUNT;
        return VIEW_CHANGED;
    }
};

class App_View_InstrumentType : public App_View_InstrumentRow {
public:
    App_View_InstrumentType(App_Tracks* _tracks, uint8_t* _instrument)
        : App_View_InstrumentRow(_tracks, _instrument, "Type   ", 12)
    {
    }

    void renderValue(App_Display* display, uint8_t col)
    {
        if (getSynth()->isWavetable) {
            strcat(display->text, "Wavetable   ");
        } else {
            strcat(display->text, "Sample      ");
        }
    }

    uint8_t update(UiKeys* keys, App_Display* display, uint8_t row, uint8_t col) override
    {
        if (keys->Right || keys->Up) {
            getSynth()->isWavetable = true;
        } else if (keys->Left || keys->Down) {
            getSynth()->isWavetable = false;
        }
        getSynth()->setNext();
        return VIEW_CHANGED;
    }
};

class App_View_InstrumentWav : public App_View_InstrumentRow {
public:
    App_View_InstrumentWav(App_Tracks* _tracks, uint8_t* _instrument)
        : App_View_InstrumentRow(_tracks, _instrument, "Wav    ", 12)
    {
    }

    void renderValue(App_Display* display, uint8_t col)
    {
        char filename[12];
        strncpy(filename, getSynth()->filename, 12);
        sprintf(display->text + strlen(display->text), "%-12s", filename);
    }

    uint8_t update(UiKeys* keys, App_Display* display, uint8_t row, uint8_t col) override
    {
        if (keys->Right || keys->Up) {
            getSynth()->setNext(+1);
        } else if (keys->Left || keys->Down) {
            getSynth()->setNext(-1);
        }
        return VIEW_CHANGED;
    }
};

class App_View_InstrumentLevel : public App_View_InstrumentRow {
public:
    App_View_InstrumentLevel(App_Tracks* _tracks, uint8_t* _instrument)
        : App_View_InstrumentRow(_tracks, _instrument, "Level  ")
    {
    }

    void renderValue(App_Display* display, uint8_t col)
    {
        if (col == 1) {
            sprintf(display->text + strlen(display->text), "%-3d%%  ", getSynth()->wave.getAmplitude());
        } else {
            if (getSynth()->wave.isMuted()) {
                strcat(display->text, "Muted");
            } else {
                strcat(display->text, ">ON  ");
            }
        }
    }

    uint8_t update(UiKeys* keys, App_Display* display, uint8_t row, uint8_t col) override
    {
        App_Instrument* synth = getSynth();
        if (col == 1) {
            if (keys->Right) {
                synth->wave.setAmplitude(synth->wave.getAmplitude() + 1);
            } else if (keys->Up) {
                synth->wave.setAmplitude(synth->wave.getAmplitude() + 10);
            } else if (keys->Left) {
                synth->wave.setAmplitude(synth->wave.getAmplitude() - 1);
            } else if (keys->Down) {
                synth->wave.setAmplitude(synth->wave.getAmplitude() - 10);
            }
        } else {
            if (synth->wave.isMuted()) {
                synth->wave.setMute(false);
            } else {
                synth->wave.setMute();
            }
        }
        return VIEW_CHANGED;
    }
};

class App_View_InstrumentEnv : public App_View_InstrumentRow {
public:
    App_View_InstrumentEnv(App_Tracks* _tracks, uint8_t* _instrument)
        : App_View_InstrumentRow(_tracks, _instrument, "Env    ")
    {
    }

    void renderValue(App_Display* display, uint8_t col)
    {
        if (col == 1) {
            sprintf(display->text + strlen(display->text), "%-5d ", getSynth()->asr.getAttack());
        } else {
            sprintf(display->text + strlen(display->text), "%-5d", getSynth()->asr.getRelease());
        }
    }

    uint8_t update(UiKeys* keys, App_Display* display, uint8_t row, uint8_t col) override
    {
        App_Instrument* synth = getSynth();
        if (col == 1) {
            if (keys->Right) {
                synth->asr.setAttack(synth->asr.getAttack() + 1);
            } else if (keys->Up) {
                synth->asr.setAttack(synth->asr.getAttack() + 10);
            } else if (keys->Left) {
                synth->asr.setAttack(synth->asr.getAttack() - 1);
            } else if (keys->Down) {
                synth->asr.setAttack(synth->asr.getAttack() - 10);
            }
        } else {
            if (keys->Right) {
                synth->asr.setRelease(synth->asr.getRelease() + 1);
            } else if (keys->Up) {
                synth->asr.setRelease(synth->asr.getRelease() + 10);
            } else if (keys->Left) {
                synth->asr.setRelease(synth->asr.getRelease() - 1);
            } else if (keys->Down) {
                synth->asr.setRelease(synth->asr.getRelease() - 10);
            }
        }
        return VIEW_CHANGED;
    }
};

class App_View_InstrumentFilter : public App_View_InstrumentRow {
public:
    App_View_InstrumentFilter(App_Tracks* _tracks, uint8_t* _instrument)
        : App_View_InstrumentRow(_tracks, _instrument, "Filter ")
    {
    }

    void renderValue(App_Display* display, uint8_t col)
    {
        // could remove leading 0
        if (col == 1) {
            sprintf(display->text + strlen(display->text), "%.3f ", getSynth()->filter.cutoff);
        } else {
            sprintf(display->text + strlen(display->text), "%.3f", getSynth()->filter.resonance);
        }
    }

    uint8_t update(UiKeys* keys, App_Display* display, uint8_t row, uint8_t col) override
    {
        App_Instrument* synth = getSynth();
        if (col == 1) {
            if (keys->Right) {
                synth->filter.setCutoff(synth->filter.cutoff + 0.001);
            } else if (keys->Up) {
                synth->filter.setCutoff(synth->filter.cutoff + 0.010);
            } else if (keys->Left) {
                synth->filter.setCutoff(synth->filter.cutoff - 0.001);
            } else if (keys->Down) {
                synth->filter.setCutoff(synth->filter.cutoff - 0.010);
            }
        } else {
            if (keys->Right) {
                synth->filter.setResonance(synth->filter.resonance + 0.001);
            } else if (keys->Up) {
                synth->filter.setResonance(synth->filter.resonance + 0.010);
            } else if (keys->Left) {
                synth->filter.setResonance(synth->filter.resonance - 0.001);
            } else if (keys->Down) {
                synth->filter.setResonance(synth->filter.resonance - 0.010);
            }
        }
        return VIEW_CHANGED;
    }
};

class App_View_Instrument : public App_View_Table {
protected:
    App_Tracks* tracks;

    uint8_t instrument = 0;

    App_View_InstrumentTrack trackField;
    App_View_InstrumentInstr instrField;
    App_View_InstrumentType typeField;
    App_View_InstrumentWav wavField;
    App_View_InstrumentLevel levelField;
    App_View_InstrumentEnv envField;
    App_View_InstrumentFilter filterField;
    App_View_TableField* fields[VIEW_INSTR_ROW * VIEW_INSTR_COL] = {
        // clang-format off
        &trackField, &trackField, NULL,
        &instrField, &instrField, NULL,
        &typeField, &typeField, NULL,
        &wavField, &wavField, NULL,
        &levelField, &levelField, &levelField,
        &envField, &envField, &envField,
        &filterField, &filterField, &filterField,
        // clang-format on
    };

public:
    App_View_Instrument(App_Tracks* _tracks)
        : App_View_Table(fields, VIEW_INSTR_ROW, VIEW_INSTR_COL)
        , tracks(_tracks)
        , trackField(_tracks, &instrument)
        , instrField(_tracks, &instrument)
        , typeField(_tracks, &instrument)
        , wavField(_tracks, &instrument)
        , levelField(_tracks, &instrument)
        , envField(_tracks, &instrument)
        , filterField(_tracks, &instrument)
    {
        initSelection();
    }

    void initDisplay(App_Display* display)
    {
        display->useColoredLabel();
        App_View_Table::initDisplay(display);
    }
};

#endif