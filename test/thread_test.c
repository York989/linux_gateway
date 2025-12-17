#include "commo_config.h"
#include "pthread.h"

uint64_t THREAD_ID = 1;




void *thread1(void *arg)
{
    printf("thread1 start\n");
}

int main(int argc, char const *argv[])
{

    if (!pthread_create(&THREAD_ID, NULL, thread1, NULL))
    {
        printf("create thread1 success\n");
    }
    else
    {
        printf("create thread1 failed\n");
    }
    while (1) {};





    return 0;
}


