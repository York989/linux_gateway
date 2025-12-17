#include "app_pool.h"

void task(void *args)
{
    /*
    printf("renwu");
    printf("任务%d", *(int *)args);
    printf会进入缓冲区，直到遇见\n或者缓冲区满才会输出
    所以一定要加上\r\n养成习惯
    */
    log_info(" 任务=%d", *(int *)args);
}


int main(int argc, char const *argv[])
{
    app_pool_init(10);

    for (size_t i = 0; i < 20; i++)
    {
        int *f = malloc(sizeof(int));
        *f     = (int)i;
        app_pool_add_task(f, task);
        // free(f);
    }

    sleep(10);
    // while (1) {}

    return 0;
}



