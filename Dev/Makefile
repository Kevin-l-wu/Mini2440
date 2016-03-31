objs := Interrupt.o Led.o Uart.o

all : $(objs)
	arm-linux-ld -r -o Dev.o $^
	
%.o : %.c
	arm-linux-gcc ${CFLAGS} -c $^
	
%.o : %.S
	arm-linux-gcc ${CFLAGS} -c $^

clean:
	rm -f *.o		
	
