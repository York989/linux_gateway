#ifndef _APP_BUF_H_
#define _APP_BUF_H_


#include "commo_config.h"
#include "pthread.h"


typedef void *app_buf_handle;

app_buf_handle app_buf_init(uint8_t capacity);

void app_buf_deinit(app_buf_handle buf);

com_status_e app_buf_write(app_buf_handle buf, char *data, uint8_t len);

com_status_e app_buf_read(app_buf_handle buf, char *pdata, uint8_t data_capacity, uint8_t *real_len);



#endif // _APP_BUF_H_
