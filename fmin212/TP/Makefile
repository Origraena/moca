CC 	= gcc -D_REENTRANT -pthread
CFLAGS  = -Wall -c
OFLAGS  = -o

OBJECTS = utils.o networkUtils.o naimiTrehel.o site.o

all: site

OBJETS : *.c
	$(CC) $(CFLAGS) *.c


clean:
	rm *.o
	rm *~


site : OBJETS
	$(CC) $(OBJECTS) $(OFLAGS) $@


