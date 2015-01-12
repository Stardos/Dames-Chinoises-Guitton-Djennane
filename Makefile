CC = gcc
CCFLAGS = -Wall

all: dames_chinoises


affichage.o: affichage.c affichage.h pion.h Makefile 
	$(CC) $(CCFLAGS) -c affichage.c

pion.o: pion.c pion.h Makefile
	$(CC) $(CCFLAGS) -c pion.c

IA.o: IA.c IA.h Makefile
	$(CC) $(CCFLAGS) -c IA.c

main.o: main.c affichage.h IA.h pion.h
	$(CC) $(CCFLAGS) -c main.c

dames_chinoises: IA.o affichage.o main.o pion.o 
	$(CC) $(CCFLAGS) -o dames_chinoises affichage.o IA.o main.o pion.o

clear: 
	rm *.o dames_chinoises affichage.o IA.o
