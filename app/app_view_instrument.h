#ifndef APP_VIEW_INSTRUMENT_H_
#define APP_VIEW_INSTRUMENT_H_

#include <app_core_renderer.h>
#include "./app_tracks.h"
#include <app_core_view_table.h>

#define VIEW_INSTR_ROW 8
#define VIEW_INSTR_COL 3
#define VIEW_INSTR_LABELS 7

class App_View_InstrumentRow : public App_View_TableLabeledRow {
protected:
    App_Tracks* tracks;
    uint8_t* instrument;

public:
    App_View_InstrumentRow(App_Tracks* _tracks, uint8_t* _instrument, const char* _label, uint8_t _cursorLen = 5)
        : App_View_TableLabeledRow(_label, _cursorLen)
        , tracks(_tracks)
        , instrument(_instrument)
    {
    }

    App_Instrument* getSynth()
    {
        return tracks->track->synths[*instrument];
    }
};

class App_View_InstrumentTrack : public App_View_InstrumentRow {
public:
    App_View_InstrumentTrack(App_Tracks* _tracks, uint8_t* _instrument)
        : App_View_InstrumentRow(_tracks, _instrument, "Track  ", 2)
    {
    }

    void renderValue(App_Renderer* renderer, uint8_t col)
    {
        sprintf(renderer->text + strlen(renderer->text), "%-2d", tracks->trackId + 1);
    }

    uint8_t update(UiKeys* keys, App_Renderer* renderer, uint8_t row, uint8_t col) override
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

    void renderValue(App_Renderer* renderer, uint8_t col)
    {
        sprintf(renderer->text + strlen(renderer->text), "%-2c", *instrument + 'A');
    }

    uint8_t update(UiKeys* keys, App_Renderer* renderer, uint8_t row, uint8_t col) override
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

    void renderValue(App_Renderer* renderer, uint8_t col)
    {
        if (getSynth()->isWavetable) {
            strcat(renderer->text, "Wavetable   ");
        } else {
            strcat(renderer->text, "Sample      ");
        }
    }

    uint8_t update(UiKeys* keys, App_Renderer* renderer, uint8_t row, uint8_t col) override
    {
        if (keys->Right || keys->Up) {
            getSynth()->isWavetable = true;
        } else if (keys->Left || keys->Down) {
            getSynth()->isWavetable = false;
        }
        getSynth()->setFirst();
        return VIEW_CHANGED;
    }
};

class App_View_InstrumentWav : public App_View_InstrumentRow {
public:
    App_View_InstrumentWav(App_Tracks* _tracks, uint8_t* _instrument)
        : App_View_InstrumentRow(_tracks, _instrument, "Wav    ", 21)
    {
    }

    void renderValue(App_Renderer* renderer, uint8_t col)
    {
        char filename[21];
        strncpy(filename, getSynth()->filename, 21);
        sprintf(renderer->text + strlen(renderer->text), "%-21s", filename);
    }

    uint8_t update(UiKeys* keys, App_Renderer* renderer, uint8_t row, uint8_t col) override
    {
        // TODO use up / down to move from block of letter A to B to C ...
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

    void renderValue(App_Renderer* renderer, uint8_t col)
    {
        if (col == 1) {
            sprintf(renderer->text + strlen(renderer->text), "%-3d%%  ", getSynth()->wave.getAmplitude());
        } else {
            if (getSynth()->wave.isMuted()) {
                strcat(renderer->text, "Muted");
            } else {
                strcat(renderer->text, ">ON  ");
            }
        }
    }

    uint8_t update(UiKeys* keys, App_Renderer* renderer, uint8_t row, uint8_t col) override
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

    void renderValue(App_Renderer* renderer, uint8_t col)
    {
        if (col == 1) {
            sprintf(renderer->text + strlen(renderer->text), "%-5d ", getSynth()->asr.getAttack());
        } else {
            sprintf(renderer->text + strlen(renderer->text), "%-5d", getSynth()->asr.getRelease());
        }
    }

    uint8_t update(UiKeys* keys, App_Renderer* renderer, uint8_t row, uint8_t col) override
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

    void renderValue(App_Renderer* renderer, uint8_t col)
    {
        // could remove leading 0
        if (col == 1) {
            sprintf(renderer->text + strlen(renderer->text), "%.3f ", getSynth()->filter.cutoff);
        } else {
            sprintf(renderer->text + strlen(renderer->text), "%.3f", getSynth()->filter.resonance);
        }
    }

    uint8_t update(UiKeys* keys, App_Renderer* renderer, uint8_t row, uint8_t col) override
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

// FIXME
// TODO if sample, then select start end
class App_View_InstrumentWavetable : public App_View_InstrumentRow {
public:
    App_View_InstrumentWavetable(App_Tracks* _tracks, uint8_t* _instrument)
        : App_View_InstrumentRow(_tracks, _instrument, "Wavtbl ", 4)
    {
    }

    bool isSelectable(uint8_t row, uint8_t col) override
    {
        return col == 1;
    }

    void renderValue(App_Renderer* renderer, uint8_t col)
    {
        if (col == 1) {
            sprintf(renderer->text + strlen(renderer->text), "%-4.1f ", getSynth()->wave.getMorph() + 1.0f);
        } else {
            sprintf(renderer->text + strlen(renderer->text), "of %d", getSynth()->wave.audioFile.wavetableCount);
        }
    }

    uint8_t update(UiKeys* keys, App_Renderer* renderer, uint8_t row, uint8_t col) override
    {
        App_Instrument* synth = getSynth();
        if (col == 1) {
            float value = synth->wave.getMorph();
            if (keys->Right) {
                synth->wave.morph(value + 0.1f);
            } else if (keys->Up) {
                synth->wave.morph(value + 1.0f);
            } else if (keys->Left) {
                synth->wave.morph(value - 0.1f);
            } else if (keys->Down) {
                synth->wave.morph(value - 1.0f);
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
    App_View_InstrumentWavetable wavetableField;
    App_View_TableField* fields[VIEW_INSTR_ROW * VIEW_INSTR_COL] = {
        // clang-format off
        &trackField, &trackField, NULL,
        &instrField, &instrField, NULL,
        &typeField, &typeField, NULL,
        &wavField, &wavField, NULL,
        &levelField, &levelField, &levelField,
        &envField, &envField, &envField,
        &filterField, &filterField, &filterField,
        &wavetableField, &wavetableField, &wavetableField,
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
        , wavetableField(_tracks, &instrument)
    {
        initSelection();
    }

    void initDisplay(App_Renderer* renderer)
    {
        renderer->useColor(0, 255, 0, 6);
        App_View_Table::initDisplay(renderer);
    }

    const char* snapshotPath = "projects/current/instruments/track%d-inst_%c.zic";

    void snapshot(App_Renderer* renderer) override
    {
        uint8_t inst = instrument;
        for (uint8_t t = 0; t < TRACK_COUNT; t++) {
            tracks->select(t);
            for (instrument = 0; instrument < INSTRUMENT_COUNT; instrument++) {
                render(renderer);
                saveFileContent(renderer->text, strlen(renderer->text), snapshotPath, t + 1, 'A' + instrument);
            }
        }
        instrument = inst;
    }

    void loadSnapshot() override
    {
        for (uint8_t t = 0; t < TRACK_COUNT; t++) {
            tracks->select(t);
            for (uint8_t i = 0; i < INSTRUMENT_COUNT; i++) {
                char filename[MAX_FILENAME];
                snprintf(filename, MAX_FILENAME, snapshotPath, t + 1, 'A' + i);
                Zic_File file(filename, "r");
                if (file.isOpen()) {
                    file.seekFromStart(7);
                    char idStr[1];
                    file.read(idStr, 1);
                    uint8_t trackId = idStr[0] - '1';
                    file.seekFromCurrent(9);
                    file.read(idStr, 1);
                    uint8_t instId = idStr[0] - 'A';
                    // printf("%s track id %d instid %d\n", filename, trackId, instId);

                    App_Instrument* instrument = tracks->tracks[trackId]->synths[instId];

                    file.seekFromCurrent(9);
                    file.read(idStr, 1);

                    file.seekFromCurrent(19);
                    char path[22];
                    file.read(path, 21);
                    path[21] = 0;
                    strtok(path, " "); // get only the part before " "

                    instrument->set(path, idStr[0] == 'W')->open();
                    // printf("filepath %s.\n", path);

                    char str5[6];
                    file.seekFromCurrent(8);
                    file.read(str5, 5);
                    str5[3] = 0;
                    instrument->wave.setAmplitude(atoi(str5));

                    file.read(str5, 5);
                    instrument->wave.setMute(str5[1] != '>');

                    file.seekFromCurrent(9);
                    file.read(str5, 5);
                    str5[5] = 0;
                    instrument->asr.setAttack(atoi(str5));

                    file.seekFromCurrent(1);
                    file.read(str5, 5);
                    str5[5] = 0;
                    instrument->asr.setRelease(atoi(str5));

                    file.seekFromCurrent(8);
                    file.read(str5, 5);
                    str5[5] = 0;
                    instrument->filter.setCutoff(strtof(str5, NULL));

                    file.seekFromCurrent(1);
                    file.read(str5, 5);
                    str5[5] = 0;
                    instrument->filter.setResonance(strtof(str5, NULL));

                    file.seekFromCurrent(8);
                    file.read(str5, 5);
                    str5[5] = 0;
                    instrument->wave.morph(strtof(str5, NULL) - 1.0f);
                    // printf("val:%s (%.5f)\n", str5, strtof(str5, NULL));

                    file.close();
                }
            }
        }
    }
};

#endif