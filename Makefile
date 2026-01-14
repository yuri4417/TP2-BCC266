CC = gcc
CFLAGS = -Wall -Wextra -g
LIBS = -lncurses

# Alvo final
exe: main.o menu.o
	$(CC) $(CFLAGS) -o exe main.o menu.o $(LIBS)

# Regra para main.c
main.o: main.c menu.h
	$(CC) $(CFLAGS) -c main.c

# Regra para menu.c
menu.o: menu.c menu.h
	$(CC) $(CFLAGS) -c menu.c

# Limpeza
clean:
	rm -f *.o exe
