#ifndef APP_STATE_BASE_H_
#define APP_STATE_BASE_H_

#include <app_core_file.h>

#define APP_STATE_BUFFER 64

#define APP_NEXT 0
#define APP_NULL 1
#define APP_STRING 2
#define APP_NUMBER 3

class App_State_Base {
public:
    void write(Zic_File* file, uint8_t type, uint16_t key = 0, void* ptr = NULL)
    {
        // Fill empty space from buffer with empty space
        // to allow to change variable format without breaking
        char buffer[APP_STATE_BUFFER];
        memset(buffer, ' ', APP_STATE_BUFFER);
        buffer[APP_STATE_BUFFER - 1] = '\n';
        int len = -1;

        if (type != APP_NEXT) {
            if (ptr == NULL || type == APP_NULL) {
                len = sprintf(buffer, "%03d --", key);
            } else if (type == APP_STRING) {
                len = sprintf(buffer, "%03d %s", key, (char*)ptr);
            } else if (type == APP_NUMBER) {
                len = sprintf(buffer, "%03d %d", key, *(uint8_t*)ptr);
            }
            if (len > 0 && len < APP_STATE_BUFFER) {
                buffer[len] = ' ';
            }
        }

        file->write(buffer, APP_STATE_BUFFER);
    }
};

#endif