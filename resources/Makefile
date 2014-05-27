OBJECTS = main.o 
 
CC = msp430-gcc
CFLAGS =-Os -Wall -g -mmcu=msp430x2011
 
all : $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o main.elf
 
%.o : %.c 
	$(CC) $(CFLAGS) -c $<

 
clean:
	rm -fr $(OBJECTS) main.elf
 
erase:
	mspdebug rf2500 "erase"
 
upload:
	mspdebug rf2500 "prog main.elf"
 
size:
	msp430-size main.elf
