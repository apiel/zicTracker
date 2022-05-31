#ifndef APP_VIEW_H_
#define APP_VIEW_H_

#include "./app_def.h"

#include <cstring>
#include <stdio.h>

class App_View
{
public:
    virtual void render(char *display) = 0;
    virtual uint8_t update(UiKeys* keys, char * display) = 0;
};

#endif
