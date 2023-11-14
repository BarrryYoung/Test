TARGET_MODULE:=simple-module

$(TARGET_MODULE)-objs := main.o device_file.o
obj-m := $(TARGET_MODULE).o
BUILDSYSTEM_DIR=/home/book/100ask_imx6ull-qemu/linux-4.9.88
PWD:=$(shell pwd)

all : 
	$(MAKE) -C $(BUILDSYSTEM_DIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(BUILDSYSTEM_DIR) M=$(PWD) clean
	rm -rf *.o *.ko \.*.cmd *.order test

load:
	insmod ./$(TARGET_MODULE).ko

unload:
	rmmod ./$(TARGET_MODULE).ko
