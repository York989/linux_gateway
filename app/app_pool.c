#include "app_pool.h"

pool_t pool;

#define MQ_NAME "/thread_pool"

void *thread_func(void *arg)
{
    while (1)
    {
        pool_task_t pool_task_msg;
        ssize_t task_len = mq_receive(pool.mq, (char *)&pool_task_msg,
                                      sizeof(pool_task_t), NULL);

        // log_info("taskelen:%d", task_len);
        if (task_len == -1)
        {
            perror("mq_receive failed");
            printf("mqdes=%ld, expected_msgsize=%zu, recv_buf=%zu\n",
                   (long)pool.mq, sizeof(pool_task_t), sizeof(pool_task_t));
            sleep(1);
            continue;
        }
        if (task_len > 0)
        {
            pool_task_msg.pool_thread_func(pool_task_msg.arg);
        }
    }
}

com_status_e app_pool_init(uint32_t pool_size)
{
    if (pool_size <= 0)
    {
        log_error("非法线程数量");
        return COM_FAIL;
    }
    pool.pool_size = pool_size; // 记录创建的线程池大小

    /* 消息队列初始化 */
    struct mq_attr attr = {
        .mq_flags   = 0,                   // 默认阻塞等待
        .mq_maxmsg  = 10,                  // 最大消息数[和线程池中线程数相等]
        .mq_msgsize = sizeof(pool_task_t), // 消息大小
    };
    pool.mq = mq_open(MQ_NAME, O_CREAT | O_RDWR, 0666, &attr);
    if (pool.mq == -1)
    {
        log_error("消息队列初始化失败");
        return COM_FAIL;
    }
    log_info("消息队列初始化成功");


    /* 创建线程 */
    pool.thread = malloc(pool_size * sizeof(pthread_t));
    for (size_t i = 0; i < pool_size; i++)
    {
        int ret = pthread_create(&pool.thread[i], NULL, thread_func, NULL);
        if (ret != 0)
        {
            log_error("线程池中线程初始化失败");
            return COM_FAIL;
        }
    }
    log_info("线程池中线程初始化成功");


    return COM_OK;
}

com_status_e app_pool_add_task(void *arg, void (*pool_thread_func)(void *))
{
    if (pool_thread_func == NULL)
    {
        log_error("添加任务失败，函数为空");
        return COM_FAIL;
    }

    pool_task_t task = {
        .arg              = arg,
        .pool_thread_func = pool_thread_func,
    };

    int ret = mq_send(pool.mq, (char *)&task, sizeof(pool_task_t), 0);
    if (ret == -1)
    {
        log_error("任务添加失败");
        return COM_FAIL;
    }
    log_info("任务添加成功");
}


void app_pool_deinit(void)
{
    for (size_t i = 0; i < pool.pool_size; i++)
    {
        pthread_cancel(pool.thread[i]);
    }
    free(pool.thread);  // 释放线程池内存
    mq_close(pool.mq);  // 关闭消息队列
    mq_unlink(MQ_NAME); // 删除消息队列

    log_info("线程池销毁成功");
}

/*
1.动态创建线程池空间
2.分配线程池
*/
