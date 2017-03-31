objs := Interrupt.o Led.o Uart.o Nand.o Lcd.o Dma.o Touch.o Dm9000.o Button.o I2c.o Timer.o
all : $(objs)
	arm-linux-ld -r -o Driver.o $^
	
%.o : %.c
	arm-linux-gcc ${CFLAGS} -c $^
	
%.o : %.S
	arm-linux-gcc ${CFLAGS} -c $^

clean:
	rm -f *.o