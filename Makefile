CC := arm-linux-gcc
LD := arm-linux-ld
OBJCOPY := arm-linux-objcopy

CFLAGS := -fno-builtin -I$(shell pwd)/Include
export CFLAGS

all: start.o main.o Core/Core.o CommonLib/CommonLib.o Driver/Driver.o Test/Test.o Net/Net.o
	arm-linux-ld -Tgboot.lds -o gboot.elf $^ -L /usr/local/arm/4.3.2/lib/gcc/arm-none-linux-gnueabi/4.3.2/armv4t -lgcc
	arm-linux-objcopy -O binary gboot.elf gboot.bin

%.o : %.S
	arm-linux-gcc -g -c $^
	
%.o : %.c
	arm-linux-gcc $(CFLAGS) -g -c $^

Core/Core.o :
	make -C Core all
	
Driver/Driver.o :
	make -C Driver all
	
CommonLib/CommonLib.o :
	make -C CommonLib all
	
Test/Test.o :
	make -C Test all

Net/Net.o :
	make -C Net all		
	
.PHONY: clean
clean:
	make -C Driver clean
	make -C CommonLib clean
	make -C Test clean
	make -C Core clean
	make -C Net clean
	rm *.o *.elf *.bin

