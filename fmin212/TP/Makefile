# Le makefile permet de générer (en fonction de la variable USE_FAULT_TOLERANT_EXT) soit l'algorithme 
# de Naimi-Trehel sans l'extension permettant une degré de tolérance aux pannes élevé, soit
# l'algorithme demandé.
#
# Les fichiers faultTolerantExt.c et naimiTrehel.c sont en conflit : ils implémentent les mêmes
# fonctions différemment et utilisent le header naimiTrehel.h

CC 	= gcc -D_REENTRANT -pthread
CFLAGS  = -Wall -c -O2
OFLAGS  = -o

# Définit les fichiers en conflit
CONFLICT = faultTolerantExt.c naimiTrehel.c
# Sélectionne tous les .c excepté ceux contenus dans CONFLICT
SRC = $(filter-out $(CONFLICT), $(wildcard *.c))
# Réalise une copie de SRC en remplaçant .c par .o
OBJECTS = $(SRC:.c=.o)

USE_FAULT_TOLERANT_EXT = 1

.PHONY: all clean

ifeq ($(USE_FAULT_TOLERANT_EXT), 1)
  OBJECTS += faultTolerantExt.o
  COMPILED = faultTolerantExt.o

all: site

faultTolerantExt.o: faultTolerantExt.c
	$(CC) $(CFLAGS) $<
else
  OBJECTS += naimiTrehel.o
  COMPILED = naimiTrehel.o

all: site

naimiTrehel.o: naimiTrehel.c
	$(CC) $(CFLAGS) $<
endif

OBJETS: $(SRC)
	$(CC) $(CFLAGS) $^

clean:
	rm *.o
	rm *~

site : $(COMPILED) OBJETS
	$(CC) $(OBJECTS) $(OFLAGS) $@


