#ifndef _COM_MSG_H_
#define _COM_MSG_H_


#include "commo_config.h"

typedef union {
    uint16_t U16_ARR[2];
    float FLOAT;
} u16_float_u;




typedef struct {
    char *connType;
    float targetSpeed;
    float targetAngle;
    char *action;
    uint8_t motorID;
    uint8_t is_start;
} com_msg_t;

void com_msg_ms2js(com_msg_t *msg, char *json);

com_status_e com_msg_js2ms(com_msg_t *msg, char *json);

#endif // _COM_MSG_H_
