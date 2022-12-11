#ifndef APP_STATE_PROJECT_H_
#define APP_STATE_PROJECT_H_

#define PROJECT_NAME_LEN 22

typedef struct App_State_Project {
    uint16_t id;
    char name[PROJECT_NAME_LEN];
} App_Project;

#endif