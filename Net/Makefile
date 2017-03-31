objs := Arp.o Tftp.o Network.o
all : $(objs)
	arm-linux-ld -r -o Net.o $^
	
%.o : %.c
	arm-linux-gcc ${CFLAGS} -c $^
	
%.o : %.S
	arm-linux-gcc ${CFLAGS} -c $^

clean:
	rm -f *.o
