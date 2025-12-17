#ifndef _APP_POOL_H_
#define _APP_POOL_H_


#include "commo_config.h"
#include "mqueue.h"
#include "pthread.h"

typedef struct {
    pthread_t *thread;
    mqd_t mq;
    uint32_t pool_size;
} pool_t;

typedef struct {
    void *arg;
    void (*pool_thread_func)(void *);
} pool_task_t;


com_status_e app_pool_init(uint32_t pool_size);
com_status_e app_pool_add_task(void *arg, void (*pool_thread_func)(void *));
void app_pool_deinit(void);

#endif // _APP_POOL_H_
