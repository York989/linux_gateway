#include "app_mqtt.h"

void mqtt_recv_cb(char *msg, int msg_len)
{
    log_info("msg_recv_cb: %.*s", msg_len, msg);
}

int main(int argc, char const *argv[])
{
    app_mqtt_init();
    app_mqtt_register_recv_cb(mqtt_recv_cb);
    int i = 0;
    while (1)
    {
        app_mqtt_send_msg("%d", i);
        i++;
        sleep(2);
    }
    return 0;
}
