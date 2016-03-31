objs := Common.o String.o Framework.o Getopt.o Error.o Printf.o

all : $(objs)
	arm-linux-ld -r -o CommonLib.o $^
	
%.o : %.c
	arm-linux-gcc ${CFLAGS} -c $^
	
%.o : %.S
	arm-linux-gcc ${CFLAGS} -c $^

clean:
	rm -f *.o
