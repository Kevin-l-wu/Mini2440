CFLAGS := -fno-builtin -I$(shell pwd)/Include
export CFLAGS

all: start.o main.o Dev/Dev.o CommonLib/CommonLib.o
	arm-linux-ld -Tgboot.lds -o gboot.elf $^ -L /usr/local/arm/4.3.2/lib/gcc/arm-none-linux-gnueabi/4.3.2 -lgcc
	arm-linux-objcopy -O binary gboot.elf gboot.bin

%.o : %.S
	arm-linux-gcc -g -c $^
	
%.o : %.c
	arm-linux-gcc $(CFLAGS) -g -c $^
	
Dev/Dev.o :
	make -C Dev all
	
CommonLib/CommonLib.o :
	make -C CommonLib all
	
.PHONY: clean
clean:
	make -C Dev clean
	make -C CommonLib clean
	rm *.o *.elf *.bin

