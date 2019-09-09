MODULE_DIR		:= /home/arthur/Projects/KernelDev/L01-LinuxTasksAndScheduler/01-TaskStruct/module
obj-m 			+= task-struct-cdev.o


all:
	  make -C /lib/modules/$(shell uname -r)/build M=$(MODULE_DIR) modules

clean:
	  make -C /lib/modules/$(shell uname -r)/build M=$(MODULE_DIR) clean

