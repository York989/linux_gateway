#include "log/log.h"

int main(int argc, char const *argv[])
{
    log_set_level(LOG_TRACE);
    log_trace("nihao");
    return 0;
}
