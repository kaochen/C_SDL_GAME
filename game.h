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

#ifndef GAME_H
#define GAME_H

#include "const.h"
#include <SDL2/SDL.h>


//move player
void movePlayer(int xPlayer, int yPlayer, int direction ,int xyGround, SDL_Surface *screen);
//move a box
void moveBox(int xPlayer, int yPlayer, int map[][NBR_OF_BLOCKS], int direction , SDL_Surface *screen);

//random number between two numbers
int random_number(int min, int max);

//add a randomGround
int addRandomGround();

//blit Ground
void blitGround(int x, int y, int typeOfGround,SDL_Surface *screen);

//pick a random ground image
//void randomGround(int x, int y, int *xyGround, SDL_Surface *screen, int type);

#endif
