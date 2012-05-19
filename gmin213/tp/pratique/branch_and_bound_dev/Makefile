CC = gcc
CFLAGS = -c
LFLAGS = -o 

A_OBJECTS =  bb.o tsp.o test.o
B_OBJECTS =  bb.o tsp.o test_bb.o
A_EXEC = phase
B_EXEC = solver_lstring_best

all: phase $(B_EXEC)

phase: $(A_OBJECTS)
	$(CC) $(LFLAGS) $(A_EXEC) $(A_OBJECTS)

$(B_EXEC): $(B_OBJECTS)
	$(CC) $(LFLAGS) $(B_EXEC) $(B_OBJECTS)

bb.o: bb.h bb.c
	$(CC) $(CFLAGS) bb.h bb.c

tsp.o: tsp.h tsp.c
	$(CC) $(CFLAGS) tsp.h tsp.c

test.o: tsp.h bb.h test.c
	$(CC) $(CFLAGS) tsp.h bb.h test.c

test_bb.o : bb.h tsp.h test_bb.c
	$(CC) $(CFLAGS) tsp.h bb.h test_bb.c
