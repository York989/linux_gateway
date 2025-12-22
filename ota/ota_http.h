#ifndef _OTA_HTTP_H_
#define _OTA_HTTP_H_


#include "commo_config.h"
#include "curl/curl.h"


void ota_http_get_text(const char *url, char **buffer);
com_status_e ota_http_dowmload_bin(const char *url, const char *path);

#endif // _OTA_HTTP_H_
