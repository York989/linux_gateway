#include "com_msg.h"


void com_msg_ms2js(com_msg_t *msg, char *json)
{
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "connType", msg->connType);
    cJSON_AddNumberToObject(root, "motorID", msg->motorID);
    cJSON_AddStringToObject(root, "action", msg->action);
    cJSON_AddNumberToObject(root, "motorSpeed", msg->motospeed);

    if (msg->status != NULL)
    {
        cJSON_AddStringToObject(root, "status", msg->status);
    }
    char *str = cJSON_Print(root);

    strcpy(json, str);
    json[strlen(str)] = '\0';

    cJSON_Delete(root);
}

com_status_e com_msg_js2ms(com_msg_t *msg, char *json)
{

    cJSON *root = cJSON_Parse(json);

    if (root == NULL)
    {
        log_error("JSON解析失败");
        return COM_FAIL;
    }

    msg->connType = strdup(cJSON_GetObjectItem(root, "connType")->valuestring);
    msg->motorID  = cJSON_GetObjectItem(root, "motorId")->valueint;
    msg->action   = strdup(cJSON_GetObjectItem(root, "action")->valuestring);

    cJSON *motor_speed = cJSON_GetObjectItem(root, "motorSpeed");
    if (motor_speed != NULL)
    {
        msg->motospeed = motor_speed->valueint;
    }

    cJSON *status = cJSON_GetObjectItem(root, "status");
    if (status != NULL)
    {
        msg->status = status->valuestring;
    }
    cJSON_Delete(root);
    return COM_OK;
}

