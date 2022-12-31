#ifndef APP_VIEW_PROJECT_H_
#define APP_VIEW_PROJECT_H_

#include "./app_midi.h"
#include "./app_state.h"
#include "./app_state_project.h"
#include "./app_tempo.h"
#include "./app_view_menu.h"
#include <app_core_renderer.h>
#include <app_core_util.h>
#include <app_core_view_table.h>

#define VIEW_PROJECT_ROW 8
#define VIEW_PROJECT_COL 3

class App_View_ProjectBpm : public App_View_TableLabeledRow {
protected:
    App_Tempo* tempo;

public:
    App_View_ProjectBpm()
        : App_View_TableLabeledRow("BPM   ", 3)
    {
        tempo = App_Tempo::getInstance();
    }

    void renderValue(App_Renderer* renderer, uint8_t col)
    {
        sprintf(renderer->text + strlen(renderer->text), "%-3d", tempo->getBpm());
    }

    uint8_t update(UiKeys* keys, App_Renderer* renderer, uint8_t row, uint8_t col) override
    {
        if (keys->Right) {
            tempo->set(tempo->getBpm() + 1);
        } else if (keys->Up) {
            tempo->set(tempo->getBpm() + 10);
        } else if (keys->Left) {
            tempo->set(tempo->getBpm() - 1);
        } else if (keys->Down) {
            tempo->set(tempo->getBpm() - 10);
        }
        return VIEW_CHANGED;
    }
};

class App_View_ProjectPlay : public App_View_TableLabeledRow {
public:
    App_View_ProjectPlay()
        : App_View_TableLabeledRow("SEQ   ", 5)
    {
    }

    void renderValue(App_Renderer* renderer, uint8_t col)
    {
        if (col == 1) {
            if (App_Tracks::getInstance()->isPlaying()) {
                strcat(renderer->text, ">PLAY ");
            } else {
                strcat(renderer->text, "PAUSE ");
            }
        } else {
            strcat(renderer->text, "RESET");
        }
    }

    uint8_t update(UiKeys* keys, App_Renderer* renderer, uint8_t row, uint8_t col) override
    {
        if (col == 1) {
            App_Tracks::getInstance()->togglePlay();
        } else {
            App_Tracks::getInstance()->reset();
        }
        return VIEW_CHANGED;
    }
};

class App_View_ProjectName : public App_View_TableLabeledRow {
protected:
    App_View_Menu* menu;

public:
    App_View_ProjectName(App_View_Menu* _menu)
        : App_View_TableLabeledRow("Name  ", PROJECT_NAME_LEN)
        , menu(_menu)
    {
    }

    void renderValue(App_Renderer* renderer, uint8_t col)
    {
        sprintf(renderer->text + strlen(renderer->text), "%-*s", PROJECT_NAME_LEN, App_State::getInstance()->project.name);
    }

    uint8_t update(UiKeys* keys, App_Renderer* renderer, uint8_t row, uint8_t col) override
    {
        menu->setView(41);
        return VIEW_CHANGED;
    }
};

class App_View_ProjectMidiApi : public App_View_TableLabeledRow {
public:
    App_View_ProjectMidiApi()
        : App_View_TableLabeledRow("Midi  ", 22)
    {
    }

    void renderValue(App_Renderer* renderer, uint8_t col)
    {
        sprintf(renderer->text + strlen(renderer->text), "%-22s", App_Midi::getInstance()->getApiName());
    }

    uint8_t update(UiKeys* keys, App_Renderer* renderer, uint8_t row, uint8_t col) override
    {
        if (keys->Right || keys->Up) {
            App_Midi::getInstance()->setNextApi(+1);
        } else if (keys->Left || keys->Down) {
            App_Midi::getInstance()->setNextApi(-1);
        }
        return VIEW_CHANGED;
    }
};

class App_View_ProjectMidi : public App_View_TableFieldCursor {
public:
    App_View_ProjectMidi()
        : App_View_TableFieldCursor(22)
    {
    }

    bool isSelectable(uint8_t row, uint8_t col) override
    {
        return col != 0;
    }

    void render(App_Renderer* renderer, uint8_t row, uint8_t col, uint8_t selectedRow, uint8_t selectedCol)
    {
        if (col == 0) {
            sprintf(renderer->text + strlen(renderer->text), "Midi%d ", row - 3);
        } else {
            App_Audio_TrackMidi* midiTrack = getMidiTrack(row);
            // FIXME need to trim name and maybe move the name...
            sprintf(renderer->text + strlen(renderer->text), "%-22s", midiTrack->midiout->getPortName(midiTrack->port).c_str());
        }
    }

    uint8_t update(UiKeys* keys, App_Renderer* renderer, uint8_t row, uint8_t col) override
    {
        int8_t direction = 0;
        if (keys->Right || keys->Up) {
            direction = +1;
        } else if (keys->Left || keys->Down) {
            direction = -1;
        }
        getMidiTrack(row)->setMidiOutPort(direction);
        return VIEW_CHANGED;
    }

    App_Audio_TrackMidi* getMidiTrack(uint8_t row)
    {
        uint8_t midiRow = row - 3;
        // FIXME need to find better way to get midi track
        if (midiRow == 1) {
            return &App_Tracks::getInstance()->track4;
        } else if (midiRow == 2) {
            return &App_Tracks::getInstance()->track5;
        } else if (midiRow == 3) {
            return &App_Tracks::getInstance()->track6;
        } else if (midiRow == 4) {
            return &App_Tracks::getInstance()->track7;
        }
        return NULL;
    }
};

class App_View_Project : public App_View_Table {
protected:
    App_View_ProjectBpm bpmField;
    App_View_ProjectPlay playField;
    App_View_ProjectName nameField;
    App_View_ProjectMidi midiField;
    App_View_ProjectMidiApi midiApiField;

    App_View_TableField* fields[VIEW_PROJECT_ROW * VIEW_PROJECT_COL] = {
        // clang-format off
        &nameField, &nameField, NULL,
        &bpmField, &bpmField, NULL,
        &playField, &playField, &playField,
        &midiApiField, &midiApiField, NULL,
        &midiField, &midiField, NULL,
        &midiField, &midiField, NULL,
        &midiField, &midiField, NULL,
        &midiField, &midiField, NULL,
        // clang-format on
    };

    App_View_Project(App_View_Menu* menu)
        : App_View_Table(fields, VIEW_PROJECT_ROW, VIEW_PROJECT_COL)
        , nameField(menu)
    {
        initSelection();
    }

public:
    static App_View_Project* instance;

    static App_View_Project* getInstance(App_View_Menu* menu)
    {
        if (!instance) {
            instance = new App_View_Project(menu);
        }
        return instance;
    }

    void preRender(App_Renderer* renderer)
    {
        renderer->useColor(0, 255, 0, 5, COLOR_MEDIUM);
        App_View_Table::preRender(renderer);
    }
};

App_View_Project* App_View_Project::instance = NULL;

#endif