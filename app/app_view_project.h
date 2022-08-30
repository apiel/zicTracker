#ifndef APP_VIEW_PROJECT_H_
#define APP_VIEW_PROJECT_H_

#include <zic_seq_tempo.h>

#include "./app_display_base.h"
#include "./app_view_table.h"
#include "./app_project.h"
#include "./app_view_menu.h"

#define VIEW_PROJECT_ROW 3
#define VIEW_PROJECT_COL 3

class App_View_ProjectBpm : public App_View_TableLabeledRow {
protected:
    Zic_Seq_Tempo<>* tempo;

public:
    App_View_ProjectBpm(Zic_Seq_Tempo<>* _tempo)
        : App_View_TableLabeledRow("BPM ", 3)
        , tempo(_tempo)
    {
    }

    void renderValue(App_Display_Base* display, uint8_t col)
    {
        sprintf(display->text + strlen(display->text), "%-3d", tempo->getBpm());
    }

    uint8_t update(UiKeys* keys, App_Display_Base* display, uint8_t row, uint8_t col) override
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
protected:
    App_Tracks* tracks;

public:
    App_View_ProjectPlay(App_Tracks* _tracks)
        : App_View_TableLabeledRow("SEQ ", 5)
        , tracks(_tracks)
    {
    }

    void renderValue(App_Display_Base* display, uint8_t col)
    {
        if (col == 1) {
            if (tracks->isPlaying()) {
                strcat(display->text, ">PLAY ");
            } else {
                strcat(display->text, "PAUSE ");
            }
        } else {
            strcat(display->text, "RESET");
        }
    }

    uint8_t update(UiKeys* keys, App_Display_Base* display, uint8_t row, uint8_t col) override
    {
        if (col == 1) {
            tracks->togglePlay();
        } else {
            tracks->reset();
        }
        return VIEW_CHANGED;
    }
};

class App_View_ProjectName : public App_View_TableLabeledRow {
protected:
    App_Project* project;
    App_View_Menu * menu;

public:
    App_View_ProjectName(App_Project* _project, App_View_Menu * _menu)
        : App_View_TableLabeledRow("Name ", PROJECT_NAME_LEN)
        , project(_project)
        , menu(_menu)
    {
    }

    void renderValue(App_Display_Base* display, uint8_t col)
    {
        sprintf(display->text + strlen(display->text), "%-*s", PROJECT_NAME_LEN, project->project.name);
    }

    uint8_t update(UiKeys* keys, App_Display_Base* display, uint8_t row, uint8_t col) override
    {
        menu->setView('J', 'N');
        return VIEW_CHANGED;
    }
};

class App_View_Project : public App_View_Table {
protected:
    App_View_ProjectBpm bpmField;
    App_View_ProjectPlay playField;
    App_View_ProjectName nameField;

    App_View_TableField* fields[VIEW_PROJECT_ROW * VIEW_PROJECT_COL] = {
        // clang-format off
        &nameField, &nameField, NULL,
        &bpmField, &bpmField, NULL,
        &playField, &playField, &playField,
        // clang-format on
    };

public:
    App_View_Project(Zic_Seq_Tempo<>* tempo, App_Tracks* tracks, App_Project* project, App_View_Menu * menu)
        : App_View_Table(fields, VIEW_PROJECT_ROW, VIEW_PROJECT_COL)
        , bpmField(tempo)
        , playField(tracks)
        , nameField(project, menu)
    {
        initSelection();
    }

    void initDisplay(App_Display_Base* display)
    {
        display->useColoredLabel();
        App_View_Table::initDisplay(display);
    }
};

#endif