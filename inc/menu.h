/*C_SDL_Game is a simple SDL Game write to practice C
Authors: kao_chen
Copyright (C) 2016 kao_chen <kaochen2[at]gmail.com>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#ifndef MENU_H
#define MENU_H

#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "const.h"
#include "game.h"

/* display menu */
void displayMenu (int levelNumber, SDL_Surface * menu,
                  Sprites tableSurface[NBR_OF_IMAGES], int map[][MAX_BLOCKS]);

/* display background menu */
void backgroundMenu (SDL_Surface * menu, Sprites tableSurface[NBR_OF_IMAGES]);

/* display shortcut in the menu */
void displayShortcut (SDL_Surface * menu);

/* display the level number */
void levelMenu (int levelNumber, SDL_Surface * menu,
                Sprites tableSurface[NBR_OF_IMAGES]);

/* count how many goals are need to complete the level */
int goalLeft (int map[][MAX_BLOCKS]);

/* count goals all ready achieve */
int nbr_of_goals (int map[][MAX_BLOCKS]);

/* Display Progress in the level */
void displayProgress (int map[][MAX_BLOCKS], SDL_Surface * menu,
                      Sprites tableSurface[NBR_OF_IMAGES]);

/* Victory or not ? */
int levelFinished (int map[][MAX_BLOCKS]);
#endif
