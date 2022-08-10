#ifndef APP_VIEW_MENU_H_
#define APP_VIEW_MENU_H_

#include "./app_display.h"
#include "./app_view.h"

#define APP_MENU_SIZE 8

typedef struct {
    const char* name;
    char key;
    App_View* view;
    char group;
    bool selected;
} Menu;

class App_View_Menu : public App_View {
protected:
    void inc(int8_t val)
    {
        // or could be:
        // currentMenu = ((currentMenu ? currentMenu : APP_MENU_SIZE) + val) % APP_MENU_SIZE;
        currentMenu = (currentMenu + val);
        if (currentMenu == 255) {
            currentMenu = APP_MENU_SIZE - 1;
        }
        currentMenu = currentMenu % APP_MENU_SIZE;
    }

    void menuInc(int8_t val)
    {
        menu[currentMenu].selected = false;
        char group = menu[currentMenu].group;
        inc(val);
        if (group != menu[currentMenu].group) {
            inc(-val);
        }
        menu[currentMenu].selected = true;
    }

    void groupInc(int8_t val)
    {
        do {
            inc(val);
        } while (!menu[currentMenu].selected);
    }

    void menuPlus()
    {
        menuInc(+1);
    }

    void menuMinus()
    {
        menuInc(-1);
    }

    void groupPlus()
    {
        groupInc(+1);
    }

    void groupMinus()
    {
        groupInc(-1);
    }

public:
    UiKeys* keys = NULL;

    Menu* menu;
    uint8_t currentMenu = 0;

    App_View_Menu(Menu* _menu)
        : menu(_menu)
    {
    }

    App_View* getView()
    {
        if (keys && keys->B) {
            return this;
        }

        App_View* view = menu[currentMenu].view;
        return view ? view : this;
    }

    void render(App_Display* display)
    {
        strcpy(display->text, "");
        for (uint8_t i = 0; i < APP_MENU_SIZE; i++) {
            if (i != 0 && menu[i].group != menu[i - 1].group) {
                strcat(display->text, "\n");
            }
            if (i == currentMenu) {
                display->setCursor(1, 1);
            }
            sprintf(display->text + strlen(display->text), " %c", menu[i].key);
        }
        sprintf(display->text + strlen(display->text), "\n\n %s", menu[currentMenu].name);
    }

    uint8_t update(UiKeys* _keys, App_Display* display)
    {
        keys = _keys;
        if (keys->B) {
            if (keys->Right) {
                menuPlus();
            } else if (keys->Left) {
                menuMinus();
            } else if (keys->Up) {
                groupMinus();
            } else if (keys->Down) {
                groupPlus();
            }
            return VIEW_CHANGED;
        }
        return VIEW_NONE;
    }
};

#endif