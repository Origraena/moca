
CC=g++
CFLAGS=-W -Wall
LDFLAGS=
SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)
EXEC=tp1

all: $(EXEC)

$(EXEC): $(OBJ)
	@$(CC) -o $@ $^ $(LDFLAGS)
	
%.o: %.c
	@$(CC) -c $< $(CFLAGS)
	
.PHONY: clean mrproper

clean:
	@rm *.o *~ *.log *.aux
	
mrproper: clean
	@rm $(EXEC)
	