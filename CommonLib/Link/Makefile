objs := LinkList.o LinkStack.o LinkQueue.o

all : $(objs)
	arm-linux-ld -r -o Link.o $^
	
%.o : %.c
	arm-linux-gcc ${CFLAGS} -g -c $^
	
%.o : %.S
	arm-linux-gcc ${CFLAGS} -c $^

clean:
	rm -f *.o