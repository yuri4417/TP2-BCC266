 CC = gcc
CFLAGS = -Wall -Wextra -g
LIBS = -lncurses

OBJS = main.o menu.o utils.o cpu.o ram.o MMU.o tabela.o

exe: $(OBJS)
	$(CC) $(CFLAGS) -o exe $(OBJS) $(LIBS)

main.o: main.c menu.h
	$(CC) $(CFLAGS) -c main.c

menu.o: menu.c menu.h
	$(CC) $(CFLAGS) -c menu.c

utils.o: utils.c utils.h structs.h
	$(CC) $(CFLAGS) -c utils.c

cpu.o: cpu.c cpu.h structs.h
	$(CC) $(CFLAGS) -c cpu.c

ram.o: ram.c ram.h structs.h
	$(CC) $(CFLAGS) -c ram.c

MMU.o: MMU.c MMU.h structs.h
	$(CC) $(CFLAGS) -c MMU.c
	
tabela.: tabela.c tabela.h structs.h
	$(CC) $(CFLAGS) -c tabela.c

.PHONY: clean
clean:
	rm -f *.o exe
