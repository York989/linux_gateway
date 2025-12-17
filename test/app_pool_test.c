#include "app_pool.h"

void task(void *args)
{
    log_info("任务%d", *(int *)args);
}


int main(int argc, char const *argv[])
{
    app_pool_init(10);

    for (size_t i = 0; i < 20; i++)
    {
        app_pool_add_task(&i, task);
    }

    sleep(1000);
    return 0;
}



