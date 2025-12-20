#ifndef _COM_MSG_H_
#define _COM_MSG_H_


#include "commo_config.h"


typedef struct {
    char *connType;
    int motorID;
    char *action;
    int motospeed;
    char *status;
} com_msg_t;

void com_msg_ms2js(com_msg_t *msg, char *json);

com_status_e com_msg_js2ms(com_msg_t *msg, char *json);

#endif // _COM_MSG_H_
