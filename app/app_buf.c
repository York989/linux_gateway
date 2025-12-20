#include "app_buf.h"

typedef struct {
    char *buf_ptr;    // 子缓冲区地址
    int buf_data_len; // 子缓冲内数据长度
    int buf_capacity; // 子缓冲区可容纳大小
} sub_buf_t;



typedef struct {
    sub_buf_t *sub_bufs[2];  // 子缓存区
    int w_index;             // 写索引
    int r_index;             // 读索引
    pthread_mutex_t w_mutex; // 写锁
    pthread_mutex_t r_mutex; // 读锁
} app_buf_t;


static sub_buf_t *sub_buf_init(int capacity)
{
    sub_buf_t *sub_buf = (sub_buf_t *)malloc(sizeof(sub_buf_t));
    if (sub_buf == NULL)
    {
        log_error("子缓冲区头创建malloc失败");
        return NULL;
    }

    sub_buf->buf_ptr = (char *)malloc(capacity); // 初始化数据区大小
    memset(sub_buf->buf_ptr, 0, capacity);
    sub_buf->buf_data_len = 0;        // 初始实际数据长度
    sub_buf->buf_capacity = capacity; // 初始化缓冲区容量

    return sub_buf;
}


app_buf_handle app_buf_init(int capacity)
{
    app_buf_t *buf = (app_buf_t *)malloc(sizeof(app_buf_t)); // 初始化缓冲区
    if (buf == NULL)
    {
        log_error("缓冲头初始化失败");
        return NULL;
    }

    buf->sub_bufs[0] = sub_buf_init(capacity); // 初始化子缓存区大小
    buf->sub_bufs[1] = sub_buf_init(capacity);

    if (buf->sub_bufs[0] == NULL || buf->sub_bufs[1] == NULL)
    {
        log_error("创建子缓冲区失败");
        free(buf);
        return NULL;
    }

    buf->w_index = 0; // 初始化写索引
    buf->r_index = 1; // 初始化读索引

    pthread_mutex_init(&buf->r_mutex, NULL); // 初始化读锁
    pthread_mutex_init(&buf->w_mutex, NULL); // 初始化写锁

    log_info("双缓冲区创建成功");
    return (app_buf_handle)buf;
}


void app_buf_deinit(app_buf_handle buf)
{
    app_buf_t *buffer = (app_buf_t *)buf;

    free(buffer->sub_bufs[0]->buf_ptr);
    free(buffer->sub_bufs[0]);

    free(buffer->sub_bufs[1]->buf_ptr);
    free(buffer->sub_bufs[1]);

    free(buffer);
}


com_status_e app_buf_write(app_buf_handle buf, char *data, uint8_t len)
{
    app_buf_t *buffer = (app_buf_t *)buf;

    if (buffer == NULL)
    {
        log_error("缓冲区不存在");
        return COM_FAIL;
    }

    pthread_mutex_lock(&buffer->w_mutex); /* 开启写锁 */
    sub_buf_t *sub_buf = buffer->sub_bufs[buffer->w_index];


    if (sub_buf->buf_capacity - sub_buf->buf_data_len < len + 1)
    {
        log_error("缓存区容量不足，剩余：%d，需要：%d：",
                  sub_buf->buf_capacity - sub_buf->buf_data_len, len + 1);
        pthread_mutex_unlock(&buffer->w_mutex); /* 关闭写锁 */
        return COM_FAIL;
    }
    // 从上次写入末尾的位置开始，先把长度写入
    sub_buf->buf_ptr[sub_buf->buf_data_len] = len;
    // 之后就是数据
    memcpy(sub_buf->buf_ptr + sub_buf->buf_data_len + 1, data, len);
    // 更新写缓存内数据长度
    sub_buf->buf_data_len += len + 1;

    pthread_mutex_unlock(&buffer->w_mutex); /* 关闭写锁 */
    log_info("写入成功");
    return COM_OK;
}

com_status_e app_buf_read(app_buf_handle buf,
                          char *pdata,
                          int data_capacity,
                          uint8_t *real_len)
{
    app_buf_t *buffer = (app_buf_t *)buf;

    pthread_mutex_lock(&buffer->r_mutex); /* 读锁 */
    sub_buf_t *sub_buf = buffer->sub_bufs[buffer->r_index];

    if (sub_buf->buf_data_len == 0)
    {
        pthread_mutex_lock(&buffer->w_mutex); // 读锁
        /* 交换缓存区 */
        int temp        = buffer->r_index;
        buffer->r_index = buffer->w_index;
        buffer->w_index = temp;
        pthread_mutex_unlock(&buffer->w_mutex);      // 解锁
        sub_buf = buffer->sub_bufs[buffer->r_index]; // 重新定位缓存区
        if (sub_buf->buf_data_len == 0)
        {
            // log_error("缓存区没有数据");
            pthread_mutex_unlock(&buffer->r_mutex);
            return COM_FAIL;
        }
    }
    // 判断接收区的大小是否足够
    uint8_t data_len = sub_buf->buf_ptr[0];
    if (data_len > data_capacity)
    {
        log_error("接收缓冲区大小不足，还需%d", data_len - data_capacity);
        pthread_mutex_unlock(&buffer->r_mutex);
        return COM_FAIL;
    }
    /* 开始接收数据 */
    memcpy(pdata, sub_buf->buf_ptr + 1, data_len);
    *real_len = data_len;

    /* 移动剩下的数据 */
    // 参数2为何+1：因为数据长度不包含在实际datalen中,+1才能到下一段数据的起始位置
    // 参数3：还是因为datalen不含数据长度的那个字节，实际取走10分，数据占9分，所以要+1回来
    memmove(sub_buf->buf_ptr, sub_buf->buf_ptr + data_len + 1, sub_buf->buf_data_len - data_len - 1);

    // 更新缓冲区大小
    sub_buf->buf_data_len -= data_len + 1;

    // 释放锁
    pthread_mutex_unlock(&buffer->r_mutex);
    log_info("读取数据成功");
    return COM_OK;
}
