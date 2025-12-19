#include "app_buf.h"


int main(int argc, char const *argv[])
{
    // app_buf_handle buffer = app_buf_init(10); // 初始化10字节，测试写入12字节

    app_buf_handle buffer = app_buf_init(10);

    app_buf_write(buffer, "12345", 5); // 包括数据长度的话，实际需要6个字节空间
    app_buf_write(buffer, "6789#", 5);

    uint8_t len;
    char buf[100];
    if (COM_OK == app_buf_read(buffer, buf, 100, &len))
    {
        log_info("读取到的数据1：%s，长度：%d", buf, len);
    }

    if (COM_OK == app_buf_read(buffer, buf, 100, &len))
    {
        log_info("读取到的数据2：%s，长度：%d", buf, len);
    }
    return 0;
}




