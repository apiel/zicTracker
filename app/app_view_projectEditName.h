#ifndef APP_VIEW_PROJECT_EDIT_NAME_H_
#define APP_VIEW_PROJECT_EDIT_NAME_H_

#include "./app_display.h"
#include "./app_project.h"
#include "./app_view_table.h"

#define VIEW_PROJECT_EDIT_NAME_ROW 3
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

class App_View_EditProjectKeyboard : public App_View_TableFieldCursor {

public:
    App_View_EditProjectKeyboard()
        : App_View_TableFieldCursor(1)
    {
    }

    void renderValue(App_Display* display, uint8_t col)
    {
        strcat(display->text, "A");
    }
};

class App_View_ProjectEditName : public App_View_Table {
protected:
    App_View_EditedProjectName nameField;
    App_View_EditProjectKeyboard kbField;

    App_View_TableField* fields[VIEW_PROJECT_EDIT_NAME_ROW * VIEW_PROJECT_EDIT_NAME_COL] = {
        // clang-format off
        &nameField, &nameField, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        &kbField, &kbField, &kbField,&kbField, &kbField, &kbField,&kbField, &kbField, &kbField, &kbField,
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