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

#ifndef LEVEL_H
#define LEVEL_H

#include "const.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
//read a level line
int readlevel(int map[][NBR_OF_BLOCKS], int level);

//display the level using map and mapGround
void displayLevel(int map[][NBR_OF_BLOCKS], int mapGround[][NBR_OF_BLOCKS], SDL_Surface *screen);

//how many level are in the file
int nbr_of_level();
#endif
