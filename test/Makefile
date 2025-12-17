cc := gcc  # 定义编译器

target := main.o
target += hello.o # 变量拼接



hello.o: hello.c hello.h
	@$(cc) -c hello.c -o hello.o


main.o: main.c hello.h
	@$(cc) -c main.c -o main.o

main: $(target)
	@$(cc) $^ -o $@
	-kdasfkhahfa
	@./main

# 特殊变量
# $@ :表示目标文件，此处就是表示main
# $^ :表示全部依赖（会自动进行去重）
# $< :表示第一个依赖

# @  :表示静默执行
# -  :忽略错误继续向后执行

-PHONY: clean  # 表示尾目标[即使有同名也会忽略]
clean:
	echo "编译完成"
	@rm -rf main *.o

# =================================================================
log := ../3rdParty/log/log.c ../3rdParty/log/log.h
cjson := ../3rdParty/cjson/cJSON.c ../3rdParty/cjson/cJSON.h
mqtt := ../app/app_mqtt.c ../app/app_mqtt.h
common := ../common/commo_config.h

log_test: log_test.c $(log)
	@$(cc) $^ -o $@ -I ../3rdParty
	@./$@
	@rm -rf $@

cjson_test: cjson_test.c $(cjson)
	@$(cc) $^ -o $@ -I ../3rdParty
	@./$@
	@rm -rf $@

mqtt_test: mqtt_test.c $(common) $(mqtt) $(log) $(cjson)
	@$(cc) $^ -o $@ -I../3rdParty -I../app -I../common -lpaho-mqtt3c
	@./$@
	@rm -rf $@


thread_test: thread_test.c $(common) $(log)
	@$(cc) $^ -o $@ -I../3rdParty -I../common -lpthread
	@./$@
	@rm -rf $@

