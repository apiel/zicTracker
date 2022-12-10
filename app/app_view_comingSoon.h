#ifndef APP_VIEW_COMING_SOON_H_
#define APP_VIEW_COMING_SOON_H_

#include <app_core_renderer.h>
#include <app_core_view.h>

class App_View_ComingSoon : public App_View {
protected:
    App_View_ComingSoon()
    {
    }

public:
    static App_View_ComingSoon* instance;

    static App_View_ComingSoon* getInstance()
    {
        if (!instance) {
            instance = new App_View_ComingSoon();
        }
        return instance;
    }

    void render(App_Renderer* renderer)
    {
        renderer->useColoredRow(0, COLOR_PLAY);
        renderer->useColor(0, 11, COLOR_HILIGHT);
        renderer->useColoredRow(2, COLOR_MEDIUM);
        renderer->useColoredRow(3, COLOR_DARK);
        strcpy(renderer->text, "Coming Soon!\n\nNeed some time/help to\nimplement more features.\n\n \\(o,o)/");
    }

    uint8_t update(UiKeys* keys, App_Renderer* renderer)
    {
        return VIEW_CHANGED;
    }
};

App_View_ComingSoon* App_View_ComingSoon::instance = NULL;

#endif