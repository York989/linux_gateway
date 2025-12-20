#ifndef _APP_DEVICE_H_
#define _APP_DEVICE_H_


#include "app_buf.h"
#include "app_modbus.h"
#include "app_mqtt.h"
#include "app_pool.h"
#include "com_msg.h"
#include "commo_config.h"

typedef struct {
    app_buf_handle upload_handle;
    app_buf_handle download_handle;
    bool is_running;
} app_dev_t;

extern app_dev_t device;

com_status_e app_dev_init(void);

com_status_e app_dev_start(void);

void app_dev_stop(void);

#endif // _APP_DEVICE_H_
