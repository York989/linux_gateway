#include "ota_version_check.h"


#define OTA_VERSION_URL "http://192.168.59.95:8000/Desktop/ota_server/ota.json"
#define OTA_CUR_VERSION "{\"major\":1,\"minor\":0,\"patch\":0,\"sha\":\"\"}"
#define OTA_BIN_URL     "http://192.168.59.95:8000/Desktop/ota_server/gateway"
#define OTA_BIN_PATH    "/usr/bin/gateway.update"

typedef struct {
    int major;
    int minor;
    int patch;
    char *sha;
} ota_info_t;

static void parse_ota_json(char *json_str, ota_info_t *ota_info)
{
    cJSON *root = cJSON_Parse(json_str);
    if (root == NULL)
    {
        log_error("ota解析JSON失败");
        return;
    }
    ota_info->major = cJSON_GetObjectItem(root, "major")->valueint;
    ota_info->minor = cJSON_GetObjectItem(root, "minor")->valueint;
    ota_info->patch = cJSON_GetObjectItem(root, "patch")->valueint;
    ota_info->sha   = strdup(cJSON_GetObjectItem(root, "sha")->valuestring);
    cJSON_Delete(root);
}

/**
 * 获取文件的SHA1哈希值（40位16进制字符串）
 * 相同文件返回相同的哈希值，可用于判断文件是否相同
 * linux命令生成：sha1sum 文件名
 * 此函数可以利用AI工具帮我们生成
 */
static char *get_file_sha(char *filepath)
{
    FILE *file = fopen(filepath, "rb");
    if (!file)
    {
        perror("Failed to open file");
        return NULL;
    }

    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA_CTX sha1;
    SHA1_Init(&sha1);

    const int bufSize     = 32768;
    unsigned char *buffer = (unsigned char *)malloc(bufSize);
    if (!buffer)
    {
        perror("Failed to allocate memory");
        fclose(file);
        return NULL;
    }

    int bytesRead;
    while ((bytesRead = fread(buffer, 1, bufSize, file)) > 0)
    {
        SHA1_Update(&sha1, buffer, bytesRead);
    }

    SHA1_Final(hash, &sha1);
    fclose(file);
    free(buffer);

    char *outputBuffer = (char *)malloc(SHA_DIGEST_LENGTH * 2 + 1);
    if (!outputBuffer)
    {
        perror("Failed to allocate memory");
        return NULL;
    }

    for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
    {
        sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
    }

    return outputBuffer;
}


void ota_version_check(void)
{
    char *ota_text_buf;
    ota_http_get_text(OTA_VERSION_URL, &ota_text_buf);
    log_info("ota_json: %s", ota_text_buf);

    ota_info_t old_ver;
    parse_ota_json(OTA_CUR_VERSION, &old_ver);
    ota_info_t new_ver;
    parse_ota_json(ota_text_buf, &new_ver);

    // 打印新老版本信息
    log_info("old_ver: %d.%d.%d %s", old_ver.major, old_ver.minor, old_ver.patch, old_ver.sha);
    log_info("new_ver: %d.%d.%d %s", new_ver.major, new_ver.minor, new_ver.patch, new_ver.sha);

    // 比较版本号
    if ((old_ver.major > new_ver.major) ||
        ((old_ver.major == new_ver.major) && (old_ver.minor > new_ver.minor)) ||
        ((old_ver.major == new_ver.major) && (old_ver.minor == new_ver.minor) && (old_ver.patch >= new_ver.patch)))
    {
        log_info("没有新版本需要更新");
        return;
    }
    else
    {
        log_info("有新版本需要更新");
    }

    // 下载新版本
    log_info("下载新版本");
    com_status_e err = ota_http_dowmload_bin(OTA_BIN_URL, OTA_BIN_PATH);
    if (err != COM_OK)
    {
        log_error("下载新版本失败");
        return;
    }

    log_info("下载新版本成功，开始校验");

    char *hash = get_file_sha(OTA_BIN_PATH);
    log_info("hash: %s", hash);
    if (strcmp(new_ver.sha, hash) != 0)
    {
        log_error("校验失败，sha1不一致");
        free(hash);
        return;
    }
    log_info("校验成功，sha1一致");
    free(hash);


    // 比较sha1
    // if (strcmp(old_ver.sha, new_ver.sha) == 0)
    // {
    //     log_info("sha1一致，不需要更新");
    // }
    // else
    // {
    //     log_info("sha1不一致，需要更新");
    // }
}



void ota_version_auto_check(void)
{
    ota_version_check();
    while (1)
    {
        time_t now   = time(NULL);
        struct tm *t = localtime(&now);
        if (t->tm_hour == 19 && t->tm_min == 56)
        {
            ota_version_check();
        }
        log_info("等待下一次");
        sleep(60);
    }
}


