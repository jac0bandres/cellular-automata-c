CC = gcc
INCLUDES = -Iinclude -g -lm
CFLAGS = `sdl2-config --cflags`
LFLAGS = `sdl2-config --libs`

clean:
	$(CC) src/main.c -o automata $(INCLUDES) $(CFLAGS) $(LFLAGS)