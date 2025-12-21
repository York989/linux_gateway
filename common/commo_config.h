#ifndef _COMMO_CONFIG_H_
#define _COMMO_CONFIG_H_


#include "cjson/cJSON.h"
#include "log/log.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"

#define ADDRESS    "tcp://192.168.59.33:1883" // 函数内MQTTClient_create不允许使用mqtt[详见注释]
#define CLIENTID   "GGYY_mqtt_client"
#define PULL_TOPIC "gateway_re"
#define PUSH_TOPIC "gateway_se"
#define _GNU_SOURCE

typedef enum {
    COM_OK = 0,
    COM_FAIL,
} com_status_e;


#endif // _COMMO_CONFIG_H_

