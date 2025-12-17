#include "commo_config.h"
#include "mqueue.h"
#include "pthread.h"

pthread_t thread_id = 2;


void *thread1(void *arg)
{
    printf("thread1 start\n");

    /* 打开消息队列 */
    struct mq_attr attr;
    attr.mq_maxmsg  = 10;   // 消息队列的最大消息数
    attr.mq_msgsize = 1024; // 消息的最大长度
    attr.mq_flags   = 0;    // 阻塞方式读取数据

    mqd_t mq = mq_open("/mq", O_CREAT | O_RDWR, 0666, &attr); // 打开消息队列

    char buf[1024] = {0};

    ssize_t recv_len = mq_receive(mq, buf, attr.mq_msgsize, NULL);
    if (recv_len == -1)
    {
        printf("mq_receive failed\n");
        return NULL;
    }
    printf("recv_len: %d, buf: %.*s\n", (int)recv_len, (int)recv_len, buf);
    /* ============================ */
    recv_len = mq_receive(mq, buf, attr.mq_msgsize, NULL);
    if (recv_len == -1)
    {
        printf("mq_receive failed\n");
        return NULL;
    }
    printf("recv_len: %d, buf: %.*s\n", (int)recv_len, (int)recv_len, buf);

    /* 关闭 */
    mq_close(mq);
    return NULL;
}

int main(int argc, char const *argv[])
{

    if (!pthread_create(&thread_id, NULL, thread1, NULL))
    {
        printf("create thread1 success\n");
    }
    else
    {
        printf("create thread1 failed\n");
    }

    /* 打开消息队列 */
    struct mq_attr attr;
    attr.mq_maxmsg  = 10;   // 消息队列的最大消息数
    attr.mq_msgsize = 1024; // 消息的最大长度
    attr.mq_flags   = 0;    // 阻塞方式读取数据

    mqd_t mq = mq_open("/mq", O_CREAT | O_RDWR, 0666, &attr); // 打开消息队列

    // printf("%x  %x", "hello", "world");


    mq_send(mq, "hello", strlen("hello"), 0); // 最后参数是优先级
    mq_send(mq, "world", strlen("world"), 0);

    /* 关闭 */
    pthread_join(thread_id, NULL);
    mq_close(mq);
    // mq_unlink("/mq");

    return 0;
}



// #include "commo_config.h"
// #include "mqueue.h"
// #include "pthread.h"

// void *thread_func(void *arg)
// {
//     struct mq_attr attr;
//     attr.mq_maxmsg  = 10;   // 容纳最大消息数
//     attr.mq_msgsize = 1024; // 每个消息最大字节数
//     attr.mq_flags   = 0;    // 0:表示以阻塞方式从消息队列中读取数据
//     mqd_t mq        = mq_open("/mq", O_RDWR | O_CREAT, 0666, &attr);

//     char buf[1024];
//     memset(buf, 0, sizeof(buf));
//     ssize_t real_len = 0;
//     // real_len         = mq_receive(mq, buf, attr.mq_msgsize, NULL);
//     if (real_len == -1)
//     {
//         printf("mq_receive error\n");
//         return NULL;
//     }

//     printf("receive: %.*s\n", real_len, buf);

//     // real_len = mq_receive(mq, buf, attr.mq_msgsize, NULL);
//     if (real_len == -1)
//     {
//         printf("mq_receive error\n");
//         return NULL;
//     }
//     printf("receive: %.*s\n", real_len, buf);

//     mq_close(mq);

//     return NULL;
// }

// int main(int argc, char const *argv[])
// {
//     pthread_t pid;

//     pthread_create(&pid, NULL, thread_func, NULL);

//     // 1. 打开一个消息队列
//     struct mq_attr attr;
//     attr.mq_maxmsg  = 10;   // 容纳最大消息数
//     attr.mq_msgsize = 1024; // 每个消息最大字节数
//     attr.mq_flags   = 0;    // 0:表示以阻塞方式从消息队列中读取数据
//     mqd_t mq        = mq_open("/mq", O_RDWR | O_CREAT, 0666, &attr);

//     // 2. 向消息队列中写入数据
//     mq_send(mq, "hello world", strlen("hello world"), 0);
//     mq_send(mq, "hello world", strlen("hello world"), 0);

//     // 3 等待子线程结束
//     pthread_join(pid, NULL);
//     mq_close(mq); // 关闭消息队列
//     // mq_unlink("/mq"); // 删除消息队列

//     return 0;
// }

