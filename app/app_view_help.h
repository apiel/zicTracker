#ifndef APP_VIEW_HELP_H_
#define APP_VIEW_HELP_H_

#include <app_core_renderer.h>
#include <app_core_view.h>

class App_View_Help : public App_View {
protected:
    App_View_Help()
    {
    }

public:
    static App_View_Help* instance;

    static App_View_Help* getInstance()
    {
        if (!instance) {
            instance = new App_View_Help();
        }
        return instance;
    }

    void render(App_Renderer* renderer)
    {
        renderer->setDefaultColor(COLOR_MEDIUM);
        renderer->useColor(0, 0, COLOR_PRIMARY);
        renderer->useColor(1, 0, COLOR_PRIMARY);
        renderer->useColor(2, 0, COLOR_PRIMARY, 3);
        renderer->useColor(3, 0, COLOR_PRIMARY);
        renderer->useColor(4, 0, COLOR_PRIMARY, 5);
        strcpy(renderer->text, "B to see the menu\nA to edit\nA+B to play/pause\nY for action\nSTART to exit\n");
    }

    uint8_t update(UiKeys* keys, App_Renderer* renderer)
    {
        return VIEW_CHANGED;
    }
};

App_View_Help* App_View_Help::instance = NULL;

#endif