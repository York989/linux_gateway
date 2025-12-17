#include "commo_config.h"
#include "pthread.h"

// 如果没有加上lock，num的值会有可能被几个线程同时取走
// 各自+1后，再覆盖回来，但是几个同时做相当于只做了一次
// num的值就会小于我们期望的值

int num = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *thread1(void *arg)
{
    for (size_t i = 0; i < 500; i++)
    {
        pthread_mutex_lock(&mutex);
        num++;
        pthread_mutex_unlock(&mutex);
    }
}

int main(int argc, char const *argv[])
{

    pthread_t pids[100];
    for (size_t i = 0; i < 100; i++)
    {
        pthread_create(&pids[i], NULL, thread1, NULL);
    }


    for (size_t i = 0; i < 100; i++)
    {
        pthread_join(pids[i], NULL); // 回收线程
    }


    printf("num = %d\n", num);
    return 0;
}
