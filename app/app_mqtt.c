#include "app_mqtt.h"

typedef struct {
    MQTTClient client;
    void (*recv_cb)(char *msg, int msg_len);
} app_mqtt_t;

static app_mqtt_t mqtt;




static void connlost(void *context, char *cause);
static int msgarrvd(void *context,
                    char *topicName,
                    int topicLen,
                    MQTTClient_message *message);
static void delivered(void *context, MQTTClient_deliveryToken dt);


com_status_e app_mqtt_init(void)
{
    /* 创建客户端 */
    if (MQTTCLIENT_SUCCESS != MQTTClient_create(&mqtt.client, ADDRESS, CLIENTID,
                                                MQTTCLIENT_PERSISTENCE_NONE, NULL))
    {
        log_error("Failed to create client");
        return COM_FAIL;
    }
    log_info("Create client success");

    /* 设置回调函数 */
    if (MQTTCLIENT_SUCCESS != MQTTClient_setCallbacks(mqtt.client, NULL,
                                                      connlost,
                                                      msgarrvd,
                                                      delivered))
    {
        log_error("Failed to set callbacks");
        return COM_FAIL;
    }
    log_info("Set callbacks success");



    /* 连接服务器 */
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer; // 默认链接参数
    if (MQTTCLIENT_SUCCESS != MQTTClient_connect(mqtt.client, &conn_opts))
    {
        log_error("Failed to connect");
        return COM_FAIL;
    }
    log_info("Connect success");


    /* 订阅主题 */
    if (MQTTCLIENT_SUCCESS != MQTTClient_subscribe(mqtt.client, PULL_TOPIC, 0))
    {
        log_error("Failed to subscribe");
        return COM_FAIL;
    }
    log_info("Subscribe success");

    return COM_OK;
}

com_status_e app_mqtt_send_msg(char *msg, int msg_len)
{
    if (MQTTCLIENT_SUCCESS != MQTTClient_publish(mqtt.client,
                                                 PUSH_TOPIC,
                                                 msg_len, msg,
                                                 0, 0, NULL))
    {
        log_error("Failed to publish");
        return COM_FAIL;
    }
    return COM_OK;
}

void app_mqtt_deinit(void)
{
    // 取消订阅
    MQTTClient_unsubscribe(mqtt.client, PUSH_TOPIC);
    // 断开连接
    MQTTClient_disconnect(mqtt.client, 10000);
    // 销毁客户端
    MQTTClient_destroy(&mqtt.client);
}

void app_mqtt_register_recv_cb(void (*cb)(char *msg, int msg_len))
{
    mqtt.recv_cb = cb;
}
// ==============================================================================
static void connlost(void *context, char *cause) // 连接断开回调函数
{
    log_error("Connection lost");

    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer; // 默认链接参数
    while (1)
    {
        if (MQTTCLIENT_SUCCESS == MQTTClient_connect(mqtt.client, &conn_opts))
        {
            log_info("Reconnect success");
            break;
        }
        log_error("Reconnect failed");
        sleep(2);
    }

    while (1)
    {
        if (MQTTCLIENT_SUCCESS == MQTTClient_subscribe(mqtt.client, "gateway", 0))
        {
            log_info("ReSubscribe success");
            break;
        }
        log_error("ReSubscribe failed");
        sleep(2);
    }
}

static int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message) // 消息到达回调函数
{
    if (mqtt.recv_cb)
    {
        mqtt.recv_cb(message->payload, message->payloadlen);
    }
    MQTTClient_freeMessage(&message); // setcallbacks中有此处说明，此处必须释放message
    MQTTClient_free(topicName);       // 释放主题名称
    return 1;
}

static void delivered(void *context, MQTTClient_deliveryToken dt) // 消息发送成功回调函数
{
    log_info("Message with token value %d delivery confirmed", dt);
}
