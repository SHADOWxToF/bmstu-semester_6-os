# Указываем объектный файл для модуля
obj-m += md1.o md2.o md3.o

# Получаем путь к исходникам ядра Linux из переменной окружения KDIR или используем значение по умолчанию
KDIR ?= /lib/modules/$(shell uname -r)/build

# Флаги
ccflags-y += -std=gnu18 -Wall

# Правило для сборки модуля
all:
	make -C $(KDIR) M=$(shell pwd) modules

# Правило для очистки директории от лишних файлов
clean:
	make -C $(KDIR) M=$(shell pwd) clean
# CURRENT = $(shell uname -r) 
# KDIR = /lib/modules/$(CURRENT)/build 
# PWD = $(shell pwd) 
# TARGET1 = main 
# obj-m := $(TARGET1).o
# all: 
# 	$(MAKE) -C $(KDIR) M=$(PWD) modules 
# clean: 
# 	@rm -f *.o .*.cmd .*.flags *.mod.c *.order 
# 	@rm -f .*.*.cmd *~ *.*~ TODO.* 
# 	@rm -fR .tmp* 
# 	@rm -rf .tmp_versions 
# disclean: clean 
# 	@rm *.ko *.symvers