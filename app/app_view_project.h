#ifndef APP_VIEW_PROJECT_H_
#define APP_VIEW_PROJECT_H_

#include <zic_seq_tempo.h>

#include "./app_project.h"
#include "./app_view_menu.h"
#include <app_core_renderer.h>
#include <app_core_util.h>
#include <app_core_view_table.h>

#define VIEW_PROJECT_ROW 3
#define VIEW_PROJECT_COL 3

class App_View_ProjectBpm : public App_View_TableLabeledRow {
protected:
    Zic_Seq_Tempo<>* tempo;

public:
    App_View_ProjectBpm(Zic_Seq_Tempo<>* _tempo)
        : App_View_TableLabeledRow("BPM  ", 3)
        , tempo(_tempo)
    {
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
protected:
    App_Tracks* tracks;

public:
    App_View_ProjectPlay(App_Tracks* _tracks)
        : App_View_TableLabeledRow("SEQ  ", 5)
        , tracks(_tracks)
    {
    }

    void renderValue(App_Renderer* renderer, uint8_t col)
    {
        if (col == 1) {
            if (tracks->isPlaying()) {
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
    App_View_Menu* menu;

public:
    App_View_ProjectName(App_Project* _project, App_View_Menu* _menu)
        : App_View_TableLabeledRow("Name ", PROJECT_NAME_LEN)
        , project(_project)
        , menu(_menu)
    {
    }

    void renderValue(App_Renderer* renderer, uint8_t col)
    {
        sprintf(renderer->text + strlen(renderer->text), "%-*s", PROJECT_NAME_LEN, project->name);
    }

    uint8_t update(UiKeys* keys, App_Renderer* renderer, uint8_t row, uint8_t col) override
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
    App_Project* project;
    Zic_Seq_Tempo<>* tempo;

    App_View_TableField* fields[VIEW_PROJECT_ROW * VIEW_PROJECT_COL] = {
        // clang-format off
        &nameField, &nameField, NULL,
        &bpmField, &bpmField, NULL,
        &playField, &playField, &playField,
        // clang-format on
    };

    App_View_Project(Zic_Seq_Tempo<>* _tempo, App_Tracks* tracks, App_Project* _project, App_View_Menu* menu)
        : App_View_Table(fields, VIEW_PROJECT_ROW, VIEW_PROJECT_COL)
        , bpmField(_tempo)
        , playField(tracks)
        , nameField(_project, menu)
        , project(_project)
        , tempo(_tempo)
    {
        initSelection();
    }

public:
    static App_View_Project* instance;

    static App_View_Project* getInstance(Zic_Seq_Tempo<>* _tempo, App_Tracks* tracks, App_Project* _project, App_View_Menu* menu)
    {
        if (!instance) {
            instance = new App_View_Project(_tempo, tracks, _project, menu);
        }
        return instance;
    }

    void initDisplay(App_Renderer* renderer)
    {
        renderer->useColor(0, 255, 0, 4, COLOR_MEDIUM);
        App_View_Table::initDisplay(renderer);
    }

    const char* snapshotPath = "projects/current/project.zic";

    void snapshot(App_Renderer* renderer) override
    {
        render(renderer);
        // printf("Saving snapshot:\n%s", renderer->text);
        saveFileContent(renderer->text, strlen(renderer->text), snapshotPath);
    }

    void loadSnapshot() override
    {
        Zic_File file(snapshotPath, "r");
        if (file.isOpen()) {
            file.seekFromStart(5);
            file.read(project->name, PROJECT_NAME_LEN);
            project->name[PROJECT_NAME_LEN - 1] = '\0';
            file.seekFromCurrent(6);
            char bpm[3];
            file.read(bpm, 3);
            // printf("yoyoyo BPM: %s\n", bpm);
            trimToNumeric(bpm);
            tempo->set(atoi(bpm));

            file.close();
        }
    }
};

App_View_Project* App_View_Project::instance = NULL;

#endif