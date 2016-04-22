#Makefile

CC=gcc
CFLAGS= `sdl2-config --cflags --libs`  -lSDL2main -lSDL2_image -lSDL2 -lSDL2_ttf `xml2-config --cflags` `xml2-config --libs`
LDFLAGS=
EXEC=SokoRobot
DEPS= inc/const.h inc/game.h inc/level.h inc/menu.h inc/sprites.h inc/slc.h
OBJS= obj/game.o obj/level.o obj/main.o obj/menu.o obj/sprites.o obj/slc.o

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC)  -o SokoRobot $(OBJS) $(CFLAGS)

obj/main.o: src/main.c $(DEPS)
	$(CC) -o obj/main.o -c src/main.c -I inc $(CFLAGS)

obj/game.o: src/game.c $(DEPS)
	$(CC) -o obj/game.o -c src/game.c -I inc $(CFLAGS)

obj/level.o: src/level.c $(DEPS)
	$(CC) -o obj/level.o -c src/level.c -I inc $(CFLAGS)

obj/menu.o: src/menu.c $(DEPS)
	$(CC) -o obj/menu.o -c src/menu.c -I inc $(CFLAGS)

obj/sprites.o: src/sprites.c $(DEPS)
	$(CC) -o obj/sprites.o -c src/sprites.c -I inc $(CFLAGS)

obj/slc.o: src/slc.c $(DEPS)
	$(CC) -o obj/slc.o -c src/slc.c -I inc $(CFLAGS)


clean:
	rm -rf obj/*.o

mrproper:
	rm -rf $(EXEC)
