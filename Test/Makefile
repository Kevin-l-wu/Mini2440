objs := TestLinkList.o

all : $(objs)
	arm-linux-ld -r -o Test.o $^
	
%.o : %.c
	arm-linux-gcc ${CFLAGS} -g -c $^
	
%.o : %.S
	arm-linux-gcc ${CFLAGS} -c $^

clean:
	rm -f *.o
