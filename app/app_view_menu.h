#ifndef APP_VIEW_MENU_H_
#define APP_VIEW_MENU_H_

#include "./app_view.h"

#define APP_MENU_SIZE 7

typedef struct {
    const char* name;
    char key;
    uint8_t view;
    bool isBase; // = false;
} Menu;

class App_View_Menu : App_View {
protected:
    void menuPlus()
    {
        currentMenu = (currentMenu + 1) % APP_MENU_SIZE;
    }

    void menuMinus()
    {
        currentMenu--;
        if (currentMenu == 255) {
            currentMenu = APP_MENU_SIZE - 1;
        }
    }

public:
    UiKeys keys;

    Menu menu[APP_MENU_SIZE] = {
        { "Track", 'T', VIEW_TRACK, true },
        { "Track Loop", 'L', VIEW_TRACK_LOOP, false },
        { "Track pattern", 'P', VIEW_TRACK_PATTERN, false },
        { "Pattern", 'P', VIEW_PATTERN, true },
        { "Pattern edit", 'E', VIEW_PATTERN_EDIT, false },
        { "Instrument", 'I', VIEW_INSTRUMENT, true },
        { "Instrument edit", 'E', VIEW_INSTRUMENT_EDIT, false },
    };
    uint8_t currentMenu = 0;

    uint8_t getView()
    {
        return menu[currentMenu].view;
    }

    void render(Display* display)
    {
        strcpy(display->text, "");
        for (uint8_t i = 0; i < APP_MENU_SIZE; i++) {
            if (i != 0 && menu[i].isBase) {
                strcat(display->text, "\n");
            }
            if (i == currentMenu) {
                sprintf(display->text + strlen(display->text), "[%c ", menu[i].key);
            } else {
                sprintf(display->text + strlen(display->text), "%c ", menu[i].key);
            }
        }
        sprintf(display->text + strlen(display->text), "\n\n%s", menu[currentMenu].name);
    }

    uint8_t update(UiKeys* keys, Display* display)
    {
        if (keys->A) {
            if (keys->Right) {
                menuPlus();
            } else if (keys->Left) {
                menuMinus();
            } else if (keys->Up) {
                do {
                    menuMinus();
                } while (!menu[currentMenu].isBase);
            } else if (keys->Down) {
                do {
                    menuPlus();
                } while (!menu[currentMenu].isBase);
            }
            render(display);
            return VIEW_CHANGED;
        }
        return VIEW_NONE;
    }
};

#endif