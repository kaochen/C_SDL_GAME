#C_SDL_Game is a simple SDL Game write to practice C
#Authors: kao_chen
#Copyright (C) 2016 kao_chen <kaochen2[at]gmail.com>

#This program is free software; you can redistribute it and/or
#modify it under the terms of the GNU General Public License
#as published by the Free Software Foundation; either version 2
#of the License, or (at your option) any later version.

#This program is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.

#You should have received a copy of the GNU General Public License
#along with this program; if not, write to the Free Software
#Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#

#Makefile

CC=gcc
CFLAGS= -std=c11 -Wall -Wextra -g `sdl2-config --cflags --libs`  -lSDL2main -lSDL2_image -lSDL2 -lSDL2_ttf `xml2-config --cflags` `xml2-config --libs` -lSDL2_gfx -lm
LDFLAGS=
OBJDIR=./obj/
BINDIR=./bin/
SRCDIR=./src/

EXEC= SokoRobot
OBJS= $(OBJDIR)level.o $(OBJDIR)level_decor.o $(OBJDIR)main.o $(OBJDIR)menu.o $(OBJDIR)menu_text.o $(OBJDIR)mouse.o $(OBJDIR)move.o $(OBJDIR)settings.o $(OBJDIR)sprites.o $(OBJDIR)slc.o

#first create the obj folder to receive *.o files :
$(shell   mkdir -p $(OBJDIR))
$(shell   mkdir -p $(BINDIR))

all: $(BINDIR)$(EXEC)


$(BINDIR)$(EXEC): $(OBJS)
	$(CC)  -o $@ $(OBJS) $(CFLAGS)

$(OBJDIR)main.o: $(SRCDIR)main.c $^
	$(CC) -o $@ -c $< -I inc $(CFLAGS)

$(OBJDIR)level.o: $(SRCDIR)level.c $^
	$(CC) -o $@ -c $< -I inc $(CFLAGS)

$(OBJDIR)level_decor.o: $(SRCDIR)level_decor.c $^
	$(CC) -o $@ -c $< -I inc $(CFLAGS)

$(OBJDIR)menu.o: src/menu.c $^
	$(CC) -o $@ -c $< -I inc $(CFLAGS)

$(OBJDIR)menu_text.o: src/menu_text.c $^
	$(CC) -o $@ -c $< -I inc $(CFLAGS)

$(OBJDIR)mouse.o: src/mouse.c $^
	$(CC) -o $@ -c $< -I inc $(CFLAGS)

$(OBJDIR)move.o: src/move.c $^
	$(CC) -o $@ -c $< -I inc $(CFLAGS)

$(OBJDIR)settings.o: src/settings.c $^
	$(CC) -o $@ -c $< -I inc $(CFLAGS)

$(OBJDIR)sprites.o: src/sprites.c $^
	$(CC) -o $@ -c $< -I inc $(CFLAGS)

$(OBJDIR)slc.o: src/slc.c $^
	$(CC) -o $@ -c $< -I inc $(CFLAGS)

.PHONY: clean mrproper

clean:
	rm -rf $(OBJDIR)
mrproper:
	rm -rf $(BINDIR)
