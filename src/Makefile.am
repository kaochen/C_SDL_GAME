AUTOMAKE_OPTIONS = gnu

AM_CPPFLAGS = -DLOCALEDIR=\""$(localedir)"\"
AM_CFLAGS= -std=c11 -Wall -Wextra -g
AM_LDFLAGS=

sdl_cflags= `pkg-config --cflags sdl2 SDL2_image SDL2_ttf SDL2_gfx`
sdl_libs= `pkg-config --libs sdl2 SDL2_image SDL2_ttf SDL2_gfx`
xml2_cflags= `pkg-config --cflags libxml-2.0 `
xml2_libs= `pkg-config --libs libxml-2.0`

override CFLAGS += $(sdl_cflags) $(xml2_cflags)
override LIBS += $(sdl_libs) $(xml2_libs) -lm

BUILT_SOURCES= \
	const.h \
	files.h \
	level.h \
	level_decor.h \
	menu.h \
	menu_text.h \
	mouse.h \
	move.h \
	settings.h \
	slc.h \
	sprites.h

bindir=$(prefix)/games/@PACKAGE@/
bin_PROGRAMS=sokorobot
sokorobot_SOURCES= \
	const.h \
	files.c \
	files.h \
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
