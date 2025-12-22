#include "ota_http.h"


size_t get_text_callback(void *ptr, size_t size, size_t nmemb, void *userdata)
{
    size_t real_size = size * nmemb;

    memcpy(userdata, ptr, real_size);
    ((char *)userdata)[real_size] = '\0';
    // log_info("real_size: %d", real_size);
    // log_info("userdata: %s", (char *)userdata);
    // log_info("ptr: %s", (char *)ptr);
    return real_size;
}

com_status_e ota_http_dowmload_bin(const char *url, const char *path)
{
    CURL *curl;
    // CURLcode result = curl_global_init(CURL_GLOBAL_ALL); // 初始一次即可，此处注释，因为在get_text内有初始化
    // if (result != CURLE_OK)
    // {
    //     printf("curl_global_init failed");
    //     return;
    // }

    curl = curl_easy_init();
    if (curl == NULL)
    {
        log_error("curl_easy_init failed");
        return COM_FAIL;
    }
    // 设置url地址
    curl_easy_setopt(curl, CURLOPT_URL, url);

    // 设置回调函数
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, fwrite);

    // 设置回调函数的参数
    FILE *fp = fopen(path, "wb");
    if (fp == NULL)
    {
        log_error("fopen failed");
        curl_easy_cleanup(curl);
        return COM_FAIL;
    }
    // char *temp = (char *)malloc(1024);
    // memset(temp, 0, 1024);

    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

    CURLcode result = curl_easy_perform(curl);
    if (result != CURLE_OK)
    {
        log_error("curl_easy_perform failed");
        curl_easy_cleanup(curl);
        // free(temp);
        return COM_FAIL;
    }
    // *buffer = temp;
    curl_easy_cleanup(curl);
    fclose(fp);
    return COM_OK;
}

void ota_http_get_text(const char *url, char **buffer)
{
    CURL *curl;
    CURLcode result = curl_global_init(CURL_GLOBAL_ALL);
    if (result != CURLE_OK)
    {
        log_error("curl_global_init failed");
        return;
    }

    curl = curl_easy_init();
    if (curl == NULL)
    {
        log_error("curl_easy_init failed");
        return;
    }
    // 设置url地址
    curl_easy_setopt(curl, CURLOPT_URL, url);

    // 设置回调函数
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, get_text_callback);

    // 设置回调函数的参数
    char *temp = (char *)malloc(1024);
    memset(temp, 0, 1024);

    curl_easy_setopt(curl, CURLOPT_WRITEDATA, temp);



    result = curl_easy_perform(curl);
    if (result != CURLE_OK)
    {
        log_error("curl_easy_perform failed");
        curl_easy_cleanup(curl);
        free(temp);
        return;
    }
    *buffer = temp;
    curl_easy_cleanup(curl);
    // log_info("temp1: %s", temp);
    // log_info("temp2: %s", temp);
    // curl_global_cleanup();
}

