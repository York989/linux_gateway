
# cc := gcc  # 定义编译器
# log :=  3rdParty/log/log.c  3rdParty/log/log.h
# cjson :=  3rdParty/cjson/cJSON.c  3rdParty/cjson/cJSON.h
# mqtt :=  app/app_mqtt.c  app/app_mqtt.h
# common :=  common/commo_config.h
# pool :=  app/app_pool.c  app/app_pool.h
# buffer :=  app/app_buf.c  app/app_buf.h
# device :=  app/app_device.c  app/app_device.h
# msg :=  common/com_msg.h  common/com_msg.c

# main: main.c $(common) $(log) $(pool) $(buffer) $(device) $(msg) $(mqtt) $(cjson)
# 	@$(cc) $^ -o $@ -I. 3rdParty -I. common -I. app
# 	@ $@
# 	@rm -rf $@


cc := gcc  # 定义编译器
log := ./3rdParty/log/log.c ./3rdParty/log/log.h
cjson := ./3rdParty/cjson/cJSON.c ./3rdParty/cjson/cJSON.h
mqtt := ./app/app_mqtt.c ./app/app_mqtt.h
common := ./common/commo_config.h ./common/com_msg.c ./common/com_msg.h
pool := ./app/app_pool.c ./app/app_pool.h
buffer := ./app/app_buf.c ./app/app_buf.h
device := ./app/app_device.c ./app/app_device.h
modbus := ./app/app_modbus.c./app/app_modbus.h


main: main.c $(common) $(log) $(pool) $(buffer) $(device) $(mqtt) $(cjson) $(modbus)
	@$(cc) $^ -o $@ -I./3rdParty -I./common -I./app -lpthread -lpaho-mqtt3c -lmodbus
	@./$@
	@rm -rf $@
