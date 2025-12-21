#ifndef _APP_MODBUS_H_
#define _APP_MODBUS_H_

#include "commo_config.h"
#include "modbus/modbus.h"

com_status_e app_modbus_init(void);
com_status_e app_modbus_readDiscReg(int slv_addr, int reg_addr, uint8_t *value);
com_status_e app_modbus_readCoilReg(int slv_addr, int reg_addr, uint8_t *value);
com_status_e app_modbus_read_single_inputREG(int slv_addr, int reg_addr, uint16_t *value);
com_status_e app_modbus_write_single_holdREG(int slv_addr, int reg_addr, uint16_t value);
com_status_e app_modbus_write_coil(int slv_addr, int reg_addr, uint16_t value);

#endif // _APP_MODBUS_H_
