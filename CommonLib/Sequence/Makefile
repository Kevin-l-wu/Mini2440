objs := SeqList.o SeqStack.o SeqQueue.o

all : $(objs)
	arm-linux-ld -r -o Sequence.o $^
	
%.o : %.c
	arm-linux-gcc ${CFLAGS} -g -c $^
	
%.o : %.S
	arm-linux-gcc ${CFLAGS} -c $^

clean:
	rm -f *.o