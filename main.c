#include "app_device.h"
#include "ota_version_check.h"
#include "signal.h"

void signal_handler(int signum)
{
    device.is_running = false;
    log_info("设备退出");
}

void app_run(void)
{
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    app_dev_init();
    app_dev_start();
    while (device.is_running)
    {
        log_info("设备运行中");
        sleep(1);
    }
    app_dev_stop();
}


int main(int argc, char const *argv[])
{


    if (argc == 1)
    {
        log_error("请传入参数：app | ota | deamon");
    }
    else if (strcmp(argv[1], "app") == 0)
    {
        log_info("app 进程启动");
        app_run();
    }
    else if (strcmp(argv[1], "ota") == 0)
    {
        log_info("ota 功能正在开发中");
        ota_version_auto_check();
    }
    else if (strcmp(argv[1], "deamon") == 0)
    {
        log_info("deamon 功能正在开发中");
    }


    return 0;
}



