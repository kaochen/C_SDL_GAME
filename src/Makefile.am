AUTOMAKE_OPTIONS = gnu

AM_CFLAGS= -std=c11 -Wall -Wextra -g `sdl2-config --cflags --libs`  -lSDL2main -lSDL2_image -lSDL2 -lSDL2_ttf `xml2-config --cflags` `xml2-config --libs` -lSDL2_gfx -lm
AM_LDFLAGS=

BUILT_SOURCES= \
	const.h \
	level.h \
	level_decor.h \
	menu.h \
	menu_text.h \
	mouse.h \
	move.h \
	settings.h \
	slc.h \
	sprites.h

bindir=$(prefix)/@PACKAGE@/
bin_PROGRAMS=SokoRobot
SokoRobot_SOURCES= \
	const.h \
	level.c \
	level.h \
	level_decor.c  \
	level_decor.h \
	menu.c \
	menu.h \
	menu_text.c \
	menu_text.h \
	mouse.c \
	mouse.h \
	move.c \
	move.h \
	settings.c \
	settings.h \
	slc.c \
	slc.h \
	sprites.c \
	sprites.h \
	main.c 
