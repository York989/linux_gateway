#include "app_device.h"


int main(int argc, char const *argv[])
{
    if (app_dev_init() == COM_FAIL)
    {
        log_error("设备初始化失败");
        return -1;
    }
    app_dev_start();
    while (device.is_running)
    {
        log_info("设备运行中");
        sleep(1);
    }
    return 0;
}



