#include "app_modbus.h"



modbus_t *ctx;

com_status_e app_modbus_init(void)
{
    ctx = modbus_new_rtu("/dev/ttyS1", 115200, "N", 8, 1);
    if (ctx == NULL)
    {
        log_error("modbus_new_rtu failed");
        return COM_FAIL;
    }

    modbus_set_debug(ctx, true);   // 打开modbus日志
    int ret = modbus_connect(ctx); // 连接modbus，打开串口
    if (ret == -1)
    {
        log_error("modbus_connect failed");
        free(ctx);
        return COM_FAIL;
    }

    log_info("modbus_init success");
    return COM_OK;
}

com_status_e app_modbus_read_single_inputREG(int slv_addr, uint16_t *value)
{
    modbus_set_slave(ctx, slv_addr);
    int ret = modbus_read_input_registers(ctx, 0, 1, value);
    if (ret == -1)
    {
        log_error("modbus_read_input_registers failed");
        return COM_FAIL;
    }
    log_info("modbus读取成功, value: %d", *value);
    return COM_OK;
}


com_status_e app_modbus_write_single_holdREG(int slv_addr, uint16_t value)
{
    modbus_set_slave(ctx, slv_addr);
    int ret = modbus_write_register(ctx, 0, value);
    if (ret == -1)
    {
        log_error("modbus_write_register failed");
        return COM_FAIL;
    }
    log_info("modbus写入成功, value: %d", value);
    return COM_OK;
}


/*
设置转速:
    设置在 保持寄存0

读取实际转速:
    存储在 输入寄存器0
*/
