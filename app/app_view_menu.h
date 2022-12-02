#ifndef APP_VIEW_MENU_H_
#define APP_VIEW_MENU_H_

#include <app_core_renderer.h>
#include <app_core_view.h>

typedef struct {
    const char* name;
    const char* shortName;
    char key;
    App_View* view;
    char group;
    bool selected;
} Menu;

class App_View_Menu : public App_View {
protected:
    Menu* menu;
    uint8_t menuSize = 0;

    void inc(int8_t val)
    {
        currentMenu = (currentMenu + val + menuSize) % menuSize;
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
    uint8_t currentMenu = 0;

    App_View_Menu(Menu* _menu, uint8_t _menuSize)
        : menu(_menu)
        , menuSize(_menuSize)
    {
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
        for (uint8_t i = 0; i < menuSize; i++) {
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

    void setView(char group, char key)
    {
        for (uint8_t i = 0; i < menuSize; i++) {
            if (menu[i].group == group && menu[i].key == key) {
                currentMenu = i;
                break;
            }
        }
    }
};

#endif