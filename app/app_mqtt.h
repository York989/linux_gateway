#ifndef _APP_MQTT_H_
#define _APP_MQTT_H_


#include "MQTTClient.h"
#include "commo_config.h"

com_status_e app_mqtt_init(void);

com_status_e app_mqtt_send_msg(char *msg, int msg_len);

void app_mqtt_register_recv_cb(void (*cb)(char *msg, int msg_len));


#endif // _APP_MQTT_H_
