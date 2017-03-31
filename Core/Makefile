objs := Framework.o Bootm.o

all : $(objs)
	arm-linux-ld -r -o Core.o $^
	
%.o : %.c
	arm-linux-gcc ${CFLAGS} -g -c $^
	
%.o : %.S
	arm-linux-gcc ${CFLAGS} -c $^

clean:
	rm -f *.o
