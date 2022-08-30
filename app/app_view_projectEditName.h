#ifndef APP_VIEW_PROJECT_EDIT_NAME_H_
#define APP_VIEW_PROJECT_EDIT_NAME_H_

#include "./app_renderer.h"
#include "./app_project.h"
#include "./app_view_menu.h"
#include "./app_view_table.h"

#define VIEW_PROJECT_EDIT_NAME_ROW 10
#define VIEW_PROJECT_EDIT_NAME_COL 10

class App_View_EditedProjectName : public App_View_TableLabeledRow {
protected:
    App_Project* project;

public:
    App_View_EditedProjectName(App_Project* _project)
        : App_View_TableLabeledRow("Name ", PROJECT_NAME_LEN)
        , project(_project)
    {
    }

    void renderValue(App_Renderer* renderer, uint8_t col)
    {
        sprintf(renderer->text + strlen(renderer->text), "%-*s", PROJECT_NAME_LEN, project->project.name);
    }

    bool isSelectable(uint8_t row, uint8_t col) override
    {
        return false;
    }
};

class App_View_EditProjectKeyboard : public App_View_TableField {
protected:
    const char* alphanum = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_-.!@$+&";
    App_Project* project;

    char getChar(uint8_t row, uint8_t col)
    {
        return alphanum[col + (row - 2) * VIEW_PROJECT_EDIT_NAME_COL];
    }

public:
    App_View_EditProjectKeyboard(App_Project* _project)
        : project(_project)
    {
    }
    bool isSelectable(uint8_t row, uint8_t col) override
    {
        return true;
    }

    void render(App_Renderer* renderer, uint8_t row, uint8_t col, uint8_t selectedRow, uint8_t selectedCol)
    {
        if (selectedRow == row && selectedCol == col) {
            renderer->setCursor(1, col == 0 ? 5 : 1);
        }
        if (col == 0) {
            strcat(renderer->text, "    ");
        }
        sprintf(renderer->text + strlen(renderer->text), " %c", getChar(row, col));
    }

    uint8_t update(UiKeys* keys, App_Renderer* renderer, uint8_t row, uint8_t col)
    {
        if (strlen(project->project.name) < PROJECT_NAME_LEN - 1) {
            sprintf(project->project.name + strlen(project->project.name), "%c", getChar(row, col));
            return VIEW_CHANGED;
        }
        return VIEW_NONE;
    }
};

class App_View_EditProjectAction : public App_View_TableField {
protected:
    App_Project* project;
    App_View_Menu* menu;

public:
    App_View_EditProjectAction(App_Project* _project, App_View_Menu* _menu)
        : project(_project)
        , menu(_menu)
    {
    }

    bool isSelectable(uint8_t row, uint8_t col) override
    {
        return true;
    }

    void render(App_Renderer* renderer, uint8_t row, uint8_t col, uint8_t selectedRow, uint8_t selectedCol)
    {
        bool selected = selectedRow == row && selectedCol == col;
        if (col == 0) {
            if (selected) {
                renderer->setCursor(9, 5);
            }
            strcat(renderer->text, "     Backspace");
        } else {
            if (selected) {
                renderer->setCursor(5, 1);
            }
            strcat(renderer->text, " Done");
        }
    }

    uint8_t update(UiKeys* keys, App_Renderer* renderer, uint8_t row, uint8_t col)
    {
        if (col == 0) {
            project->project.name[strlen(project->project.name) - 1] = '\0';
            return VIEW_CHANGED;
        } else {
            menu->setView('J', 'J');
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

public:
    App_View_ProjectEditName(App_Project* project, App_View_Menu* menu)
        : App_View_Table(fields, VIEW_PROJECT_EDIT_NAME_ROW, VIEW_PROJECT_EDIT_NAME_COL)
        , nameField(project)
        , kbField(project)
        , actionField(project, menu)
    {
        initSelection();
    }

    void initDisplay(App_Renderer* renderer)
    {
        renderer->useColoredLabel();
        App_View_Table::initDisplay(renderer);
    }
};

#endif