objs := NandInit.o Common.o String.o GetoptTemp.o Error.o Malloc.o DLinkList.o Template.o Getopt.o Sequence/Sequence.o Link/Link.o

all : $(objs)
	arm-linux-ld -r -o CommonLib.o $^
	
%.o : %.c
	arm-linux-gcc ${CFLAGS} -g -c $^
	
Sequence/Sequence.o :
	make -C Sequence all
	
Link/Link.o :
	make -C Link all
	
%.o : %.S
	arm-linux-gcc ${CFLAGS} -c $^

clean:
	make -C Sequence clean
	make -C Link clean
	rm -f *.o
