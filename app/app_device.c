#include "app_device.h"

static void mqtt_recv_cb_func(char *msg, int msg_len);
static void download_task(void *args);
static void upload_task(void *args);

app_dev_t device;

com_status_e app_dev_init(void)
{
    com_status_e ret = COM_FAIL;

    /* 初始化mqtt */
    ret = app_mqtt_init();
    if (ret == COM_FAIL)
    {
        log_error("MQTT初始化失败");
        return ret;
    }

    /* 初始化线程池 */
    ret = app_pool_init(10);
    if (ret == COM_FAIL)
    {
        log_error("线程池初始化失败");
        app_mqtt_deinit();
        return ret;
    }

    /* 初始化下行缓冲区 */
    device.download_handle = app_buf_init(1024);
    if (device.download_handle == NULL)
    {
        log_error("下行缓冲区初始化失败");
        app_mqtt_deinit();
        app_pool_deinit();
        return COM_FAIL;
    }

    /* 初始化上行缓冲区 */
    device.upload_handle = app_buf_init(1024);
    if (device.upload_handle == NULL)
    {
        log_error("上行缓冲区初始化失败");
        app_mqtt_deinit();
        app_pool_deinit();
        app_buf_deinit(device.download_handle);
        return COM_FAIL;
    }

    /* 初始化modbus */
    ret = app_modbus_init();
    if (ret == COM_FAIL)
    {
        log_error("modbus初始化失败");
        app_mqtt_deinit();
        app_pool_deinit();
        app_buf_deinit(device.download_handle);
        app_buf_deinit(device.upload_handle);
        return ret;
    }


    return COM_OK;
}

com_status_e app_dev_start(void)
{
    com_status_e ret = COM_FAIL;
    /* 注册MQTT接收回调函数 */
    app_mqtt_register_recv_cb(mqtt_recv_cb_func);

    /* 创建上行发送读取线程 */
    ret = app_pool_add_task(NULL, upload_task);
    ret = app_pool_add_task(NULL, upload_task);
    if (ret == COM_FAIL)
    {
        log_error("创建上行任务失败");
        return ret;
    }


    /* 创建下行接收读取线程 */
    ret = app_pool_add_task(NULL, download_task);
    ret = app_pool_add_task(NULL, download_task);
    if (ret == COM_FAIL)
    {
        log_error("创建下行任务失败");
        return ret;
    }
    device.is_running = true;
    log_info("设备创建成功");
    return COM_OK;
}

void app_dev_stop(void)
{
    app_mqtt_deinit();
    app_pool_deinit();
    app_buf_deinit(device.download_handle);
    app_buf_deinit(device.upload_handle);
}


static void mqtt_recv_cb_func(char *msg, int msg_len)
{
    log_info("MQTT收到数据");
    log_info("收到%d数据:\n%.*s", msg_len, msg_len, msg);
    app_buf_write(device.download_handle, msg, msg_len);
}


static void download_task(void *args)
{
    char data[1024]  = {0};
    uint8_t real_len = 0;
    while (1)
    {
        if (COM_FAIL == app_buf_read(device.download_handle, data, sizeof(data), &real_len))
        {
            continue; // 如果没收到数据，回到while循环开始处重新收集
        }

        log_info("收到数据：%.*s", real_len, data);
        com_msg_t msg;
        com_msg_js2ms(&msg, data);
        // 分别打印解析后的数据,内的所有成员
        if (strcmp(msg.connType, "RS485") == 0)
        {
            if (strcmp(msg.action, "set") == 0)
            {
                log_info("收到设置指令");
                log_info("%s, %d, %s, %d", msg.connType, msg.motorID, msg.action, msg.motospeed);
                app_modbus_write_single_holdREG(msg.motorID, msg.motospeed);
            }
            else if (strcmp(msg.action, "get") == 0)
            {
                log_info("收到获取指令");
                com_status_e err = app_modbus_read_single_inputREG(msg.motorID, (uint16_t *)&msg.motospeed);
                if (err == COM_OK)
                {
                    msg.status = "ok";
                }
                else
                {
                    msg.status = "fail";
                }
                // 假设读到数据：
                com_msg_ms2js(&msg, data);
                app_buf_write(device.upload_handle, data, strlen(data));
            }
        }
        else if (strcmp(msg.connType, "lora") == 0)
        {
            //...
        }
    }
}

static void upload_task(void *args)
{
    char data[1024]  = {0};
    uint8_t real_len = 0;
    while (1)
    {
        com_status_e ret = app_buf_read(device.upload_handle, data, sizeof(data), &real_len);
        if (ret == COM_OK)
        {
            data[real_len] = '\0';
            app_mqtt_send_msg(data, real_len);
        }
    }
}


