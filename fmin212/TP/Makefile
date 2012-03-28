CC 	= gcc -D_REENTRANT -pthread
CFLAGS  = -Wall -c -O2
OFLAGS  = -o

OBJECTS = utils.o networkUtils.o naimiTrehel.o faultTolerantExt.o site.o

all: site

OBJETS : *.c
	$(CC) $(CFLAGS) *.c


clean:
	rm *.o
	rm *~


site : OBJETS
	$(CC) $(OBJECTS) $(OFLAGS) $@


