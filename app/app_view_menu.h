#ifndef APP_VIEW_MENU_H_
#define APP_VIEW_MENU_H_

#include <app_core_renderer.h>
#include <app_core_view.h>

#ifndef APP_MENU_SIZE
#error "Define APP_MENU_SIZE before including app_view_menu.h"
#endif

typedef struct {
    uint8_t id;
    uint8_t group;
    const char* name;
    const char* shortName;
    App_View* view;
} Menu;

class App_View_Menu : public App_View {
protected:
    bool selected[APP_MENU_SIZE];
    Menu* menu;

    void inc(int8_t val)
    {
        currentMenu = (currentMenu + val + APP_MENU_SIZE) % APP_MENU_SIZE;
    }

    void menuInc(int8_t val)
    {
        selected[currentMenu] = false;
        uint8_t group = menu[currentMenu].group;
        inc(val);
        if (group != menu[currentMenu].group) {
            inc(-val);
        }
        selected[currentMenu] = true;
    }

    void groupInc(int8_t val)
    {
        do {
            inc(val);
        } while (!selected[currentMenu]);
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
    uint8_t currentMenu = 0;

    App_View_Menu(Menu* _menu)
        : menu(_menu)
    {

    }

    void initMenu()
    {
        uint8_t previousGroup = 0;
        for (uint8_t i = 0; i < APP_MENU_SIZE; i++) {
            selected[i] = previousGroup != menu[i].group;
            printf("Menu %d: %s => %d\n", i, menu[i].name, selected[i]);
            previousGroup = menu[i].group;
        }
    }

    App_View* getView()
    {
        if (keys && keys->Menu) {
            // return Menu view
            return this;
        }

        App_View* view = menu[currentMenu].view;
        return view ? view : this;
    }

    void render(App_Renderer* renderer)
    {
        renderer->setDefaultColor(COLOR_MEDIUM);
        strcpy(renderer->text, "");
        uint8_t row = 0, col = 0;
        for (uint8_t i = 0; i < APP_MENU_SIZE; i++) {
            if (i != 0 && menu[i].group != menu[i - 1].group) {
                strcat(renderer->text, "\n");
                row++;
                col = 0;
            }
            if (i == currentMenu) {
                renderer->setCursor(1, 1);
            }
            renderer->useColor(row, col + 1, COLOR_PRIMARY);
            sprintf(renderer->text + strlen(renderer->text), " %s", menu[i].shortName);
            col += strlen(menu[i].shortName) + 1;
        }
        renderer->useColor(row + 2, 1, COLOR_HILIGHT, 2);
        sprintf(renderer->text + strlen(renderer->text), "\n\n >> %s", menu[currentMenu].name);
    }

    uint8_t update(UiKeys* _keys, App_Renderer* renderer)
    {
        keys = _keys;
        if (keys->Menu) {
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

    void setView(uint8_t id)
    {
        for (uint8_t i = 0; i < APP_MENU_SIZE; i++) {
            if (menu[i].id == id) {
                currentMenu = i;
                return;
            }
        }
    }
};

#endif