#ifndef APP_MENU_GROUP_H_
#define APP_MENU_GROUP_H_

#include "./app_def.h"
#include "./app_menu_item.h"

#define MAX_MENU_ITEMS 3

class App_Menu_Group {
protected:
    App_Menu_Item* items[MAX_MENU_ITEMS] = { nullptr, nullptr, nullptr };

public:
    void render(UiKeys* keys, char* display)
    {
    }
};

#endif