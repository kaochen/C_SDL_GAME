#Makefile

CC=gcc
CFLAGS= -std=c11 -Wall -Wextra -g `sdl2-config --cflags --libs`  -lSDL2main -lSDL2_image -lSDL2 -lSDL2_ttf `xml2-config --cflags` `xml2-config --libs` -lSDL2_gfx
LDFLAGS=
OBJDIR=obj
EXEC=SokoRobot
DEPS= inc/const.h inc/level.h inc/level_decor.h inc/menu.h inc/sprites.h inc/slc.h
OBJS= $(OBJDIR)/level.o $(OBJDIR)/level_decor.o $(OBJDIR)/main.o $(OBJDIR)/menu.o $(OBJDIR)/menu_text.o $(OBJDIR)/settings.o $(OBJDIR)/sprites.o $(OBJDIR)/slc.o

#first create the obj folder to receive *.o files :
$(shell   mkdir -p $(OBJDIR))

all: $(EXEC)


$(EXEC): $(OBJS)
	$(CC)  -o SokoRobot $(OBJS) $(CFLAGS)

$(OBJDIR)/main.o: src/main.c $(DEPS)
	$(CC) -o $(OBJDIR)/main.o -c src/main.c -I inc $(CFLAGS)

$(OBJDIR)/level.o: src/level.c $(DEPS)
	$(CC) -o $(OBJDIR)/level.o -c src/level.c -I inc $(CFLAGS)

$(OBJDIR)/level_decor.o: src/level_decor.c $(DEPS)
	$(CC) -o $(OBJDIR)/level_decor.o -c src/level_decor.c -I inc $(CFLAGS)

$(OBJDIR)/menu.o: src/menu.c $(DEPS)
	$(CC) -o $(OBJDIR)/menu.o -c src/menu.c -I inc $(CFLAGS)

$(OBJDIR)/menu_text.o: src/menu_text.c $(DEPS)
	$(CC) -o $(OBJDIR)/menu_text.o -c src/menu_text.c -I inc $(CFLAGS)

$(OBJDIR)/settings.o: src/settings.c $(DEPS)
	$(CC) -o $(OBJDIR)/settings.o -c src/settings.c -I inc $(CFLAGS)

$(OBJDIR)/sprites.o: src/sprites.c $(DEPS)
	$(CC) -o $(OBJDIR)/sprites.o -c src/sprites.c -I inc $(CFLAGS)

$(OBJDIR)/slc.o: src/slc.c $(DEPS)
	 $(CC) -o $(OBJDIR)/slc.o -c src/slc.c -I inc $(CFLAGS)


clean:
	rm -rf $(OBJDIR)/*.o

mrproper:
	rm -rf $(EXEC)
