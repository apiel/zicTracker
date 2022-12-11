#ifndef APP_VIEW_PROJECT_EDIT_NAME_H_
#define APP_VIEW_PROJECT_EDIT_NAME_H_

#include "./app_project.h"
#include "./app_state.h"
#include "./app_view_menu.h"
#include <app_core_renderer.h>
#include <app_core_view_table.h>

#define VIEW_PROJECT_EDIT_NAME_ROW 10
#define VIEW_PROJECT_EDIT_NAME_COL 10

class App_View_EditedProjectName : public App_View_TableLabeledRow {
protected:
public:
    App_View_EditedProjectName()
        : App_View_TableLabeledRow("Name ", PROJECT_NAME_LEN)
    {
    }

    void renderValue(App_Renderer* renderer, uint8_t col)
    {
        sprintf(renderer->text + strlen(renderer->text), "%-*s", PROJECT_NAME_LEN, App_State::getInstance()->project.name);
    }

    bool isSelectable(uint8_t row, uint8_t col) override
    {
        return false;
    }
};

class App_View_EditProjectKeyboard : public App_View_TableField {
protected:
    const char* alphanum = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_-.!@$+&";

    char getChar(uint8_t row, uint8_t col)
    {
        return alphanum[col + (row - 2) * VIEW_PROJECT_EDIT_NAME_COL];
    }

public:
    App_View_EditProjectKeyboard()
    {
    }
    bool isSelectable(uint8_t row, uint8_t col) override
    {
        return true;
    }

    void selected(App_Renderer* renderer, uint8_t row, uint8_t col) override
    {
        renderer->setCursor(1, col == 0 ? 5 : 1);
    }

    void render(App_Renderer* renderer, uint8_t row, uint8_t col, uint8_t selectedRow, uint8_t selectedCol)
    {
        if (col == 0) {
            strcat(renderer->text, "    ");
        }
        sprintf(renderer->text + strlen(renderer->text), " %c", getChar(row, col));
    }

    uint8_t update(UiKeys* keys, App_Renderer* renderer, uint8_t row, uint8_t col)
    {
        App_Project* project = &App_State::getInstance()->project;
        if (strlen(project->name) < PROJECT_NAME_LEN - 1) {
            sprintf(project->name + strlen(project->name), "%c", getChar(row, col));
            return VIEW_CHANGED;
        }
        return VIEW_NONE;
    }
};

class App_View_EditProjectAction : public App_View_TableField {
protected:
    App_View_Menu* menu;

public:
    App_View_EditProjectAction(App_View_Menu* _menu)
        : menu(_menu)
    {
    }

    bool isSelectable(uint8_t row, uint8_t col) override
    {
        return true;
    }

    void selected(App_Renderer* renderer, uint8_t row, uint8_t col) override
    {
        if (col == 0) {
            renderer->setCursor(9, 5);
        } else {
            renderer->setCursor(5, 1);
        }
    }

    void render(App_Renderer* renderer, uint8_t row, uint8_t col, uint8_t selectedRow, uint8_t selectedCol)
    {
        strcat(renderer->text, col == 0 ? "     Backspace" : " Done");
    }

    uint8_t update(UiKeys* keys, App_Renderer* renderer, uint8_t row, uint8_t col)
    {
        if (col == 0) {
            App_Project* project = &App_State::getInstance()->project;
            project->name[strlen(project->name) - 1] = '\0';
            return VIEW_CHANGED;
        } else {
            menu->setView(40);
            return VIEW_CHANGED;
        }
        return VIEW_NONE;
    }
};

class App_View_ProjectEditName : public App_View_Table {
protected:
    App_View_EditedProjectName nameField;
    App_View_EditProjectKeyboard kbField;
    App_View_EditProjectAction actionField;

    App_View_TableField* fields[VIEW_PROJECT_EDIT_NAME_ROW * VIEW_PROJECT_EDIT_NAME_COL] = {
        // clang-format off
        &nameField, &nameField, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        &kbField, &kbField, &kbField, &kbField, &kbField, &kbField, &kbField, &kbField, &kbField, &kbField,
        &kbField, &kbField, &kbField, &kbField, &kbField, &kbField, &kbField, &kbField, &kbField, &kbField,
        &kbField, &kbField, &kbField, &kbField, &kbField, &kbField, &kbField, &kbField, &kbField, &kbField,
        &kbField, &kbField, &kbField, &kbField, &kbField, &kbField, &kbField, &kbField, &kbField, &kbField,
        &kbField, &kbField, &kbField, &kbField, &kbField, &kbField, &kbField, &kbField, &kbField, &kbField,
        &kbField, &kbField, &kbField, &kbField, &kbField, &kbField, &kbField, &kbField, &kbField, &kbField,
        &kbField, &kbField, &kbField, &kbField, &kbField, &kbField, &kbField, &kbField, &kbField, &kbField,
        &actionField, NULL, NULL, NULL, NULL, &actionField, NULL, NULL, NULL, NULL,
        // clang-format on
    };

    App_View_ProjectEditName(App_View_Menu* menu)
        : App_View_Table(fields, VIEW_PROJECT_EDIT_NAME_ROW, VIEW_PROJECT_EDIT_NAME_COL)
        , actionField(menu)
    {
        initSelection();
    }

public:
    static App_View_ProjectEditName* instance;

    static App_View_ProjectEditName* getInstance(App_View_Menu* menu)
    {
        if (!instance) {
            instance = new App_View_ProjectEditName(menu);
        }
        return instance;
    }

    void preRender(App_Renderer* renderer)
    {
        renderer->useColor(0, 1, 0, 4, COLOR_MEDIUM);
        App_View_Table::preRender(renderer);
    }
};

App_View_ProjectEditName* App_View_ProjectEditName::instance = NULL;

#endif