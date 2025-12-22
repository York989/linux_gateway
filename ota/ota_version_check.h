#ifndef _OTA_VERSION_CHECK_H_
#define _OTA_VERSION_CHECK_H_

#include "ota_http.h"
#include "time.h"
#include <openssl/sha.h>
void ota_version_check(void);
void ota_version_auto_check(void);

#endif // _OTA_VERSION_CHECK_H_
