#include "cjson/cJSON.h"
#include "stdio.h"
#include "stdlib.h"

void json_prase(void)
{
    char *json  = "{\"name\":\"John\", \"age\":30, \"city\":\"New York\"}";
    cJSON *root = cJSON_Parse(json);
    if (root != NULL)
    {
        cJSON *name = cJSON_GetObjectItem(root, "name");
        cJSON *age  = cJSON_GetObjectItem(root, "age");
        cJSON *city = cJSON_GetObjectItem(root, "city");
        if (name != NULL && name->type == cJSON_String)
        {
            printf("Name: %s\n", name->valuestring);
        }
        if (age != NULL && age->type == cJSON_Number)
        {
            printf("Age: %d\n", age->valueint);
        }
        if (city != NULL && city->type == cJSON_String)
        {
            printf("City: %s\n", city->valuestring);
        }
        cJSON_Delete(root);
    }
}

void json_build(void)
{
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "name", "John");
    cJSON_AddNumberToObject(root, "age", 30);
    cJSON_AddStringToObject(root, "city", "New York");
    char *json = cJSON_Print(root);
    printf("%s\n", json);
    cJSON_Delete(root);
    free(json);
}

int main(void)
{
    json_prase();
    json_build();
    return 0;
}
