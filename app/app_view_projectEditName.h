#ifndef APP_VIEW_PROJECT_EDIT_NAME_H_
#define APP_VIEW_PROJECT_EDIT_NAME_H_

#include "./app_display.h"
#include "./app_project.h"
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

    void renderValue(App_Display* display, uint8_t col)
    {
        sprintf(display->text + strlen(display->text), "%-*s", PROJECT_NAME_LEN, project->project.name);
    }

    bool isSelectable(uint8_t row, uint8_t col) override
    {
        return false;
    }
};

class App_View_EditProjectKeyboard : public App_View_TableField {
protected:
    const char* alphanum = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_-.!@$+&";

public:
    bool isSelectable(uint8_t row, uint8_t col) override
    {
        return true;
    }

    void render(App_Display* display, uint8_t row, uint8_t col, uint8_t selectedRow, uint8_t selectedCol)
    {
        if (selectedRow == row && selectedCol == col) {
            display->setCursor(1, col == 0 ? 5 : 1);
        }
        if (col == 0) {
            strcat(display->text, "    ");
        }
        // strcat(display->text, " A");
        sprintf(display->text + strlen(display->text), " %c", alphanum[col + (row - 2) * VIEW_PROJECT_EDIT_NAME_COL]);
    }
};

class App_View_EditProjectAction : public App_View_TableField {
public:
    bool isSelectable(uint8_t row, uint8_t col) override
    {
        return true;
    }

    void render(App_Display* display, uint8_t row, uint8_t col, uint8_t selectedRow, uint8_t selectedCol)
    {
        bool selected = selectedRow == row && selectedCol == col;
        if (col == 0) {
            if (selected) {
                display->setCursor(9, 5);
            }
            strcat(display->text, "     Backspace");
        } else {
            if (selected) {
                display->setCursor(5, 1);
            }
            strcat(display->text, " Done");
        }
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
    App_View_ProjectEditName(App_Project* project)
        : App_View_Table(fields, VIEW_PROJECT_EDIT_NAME_ROW, VIEW_PROJECT_EDIT_NAME_COL)
        , nameField(project)
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