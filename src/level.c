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

#ifndef LEVEL_C
#define LEVEL_C

#include "../inc/level.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../inc/const.h"
#include "../inc/game.h"

/* display level on the screen */
void displayLevel(Square grid[][MAX_BLOCKS], SDL_Surface * screen,
		  Sprites tableSurface[NBR_OF_IMAGES])
{
    /* set a goal position */
    SDL_Rect goalPos;

    /* blit surfaces depending of its destiny */
    int x = 0, y = 0;
    for (x = 0; x < X_BLOCKS; x++) {
	for (y = 0; y < Y_BLOCKS; y++) {

	    /* blit all blocks depending on grid mainRole */
	    switch (grid[x][y].mainRole) {
	    case GROUND:
		blitGround(x, y, grid, screen, tableSurface);
		break;
	    case BOX:
		moveBox(x, y, grid, STILL, screen, tableSurface);
		if (grid[x][y].objectType != EMPTY) {
		    SDL_BlitSurface(tableSurface[GOAL_IMAGE].image, NULL,
				    screen, &goalPos);
		}
		break;
	    case BOX_OK:
		moveBox(x, y, grid, STILL, screen, tableSurface);
		break;
	    case WALL:
		blitWalls(x, y, grid, screen, tableSurface);
		break;
	    case GOAL:
		goalPos.x = x * BOX_SIZE;
		goalPos.y = y * BOX_SIZE;
		blitGround(x, y, grid, screen, tableSurface);
		SDL_BlitSurface(tableSurface[GOAL_IMAGE].image, NULL,
				screen, &goalPos);
		break;
	    case PLAYER:
		blitGround(x, y, grid, screen, tableSurface);
		blitPlayer(x, y, STILL, grid, screen, tableSurface);
		break;
       }

      /* blit all blocks depending on grid mainRole */
      switch (grid[x][y].subRole) {
	    case OUTSIDE:
		goalPos.x = x * BOX_SIZE;
		goalPos.y = y * BOX_SIZE;
		SDL_BlitSurface(tableSurface[OUTSIDE_IMAGE].image, NULL,
				screen, &goalPos);
		break;
	    case OUTSIDE2:
		goalPos.x = x * BOX_SIZE;
		goalPos.y = y * BOX_SIZE;
		SDL_BlitSurface(tableSurface[OUTSIDE2_IMAGE].image, NULL,
				screen, &goalPos);
		break;
	    case OUTSIDE3:
		goalPos.x = x * BOX_SIZE;
		goalPos.y = y * BOX_SIZE;
		SDL_BlitSurface(tableSurface[OUTSIDE3_IMAGE].image, NULL,
				screen, &goalPos);
		break;
       case OUTSIDE4:
		goalPos.x = x * BOX_SIZE;
		goalPos.y = y * BOX_SIZE;
		SDL_BlitSurface(tableSurface[OUTSIDE4_IMAGE].image, NULL,
				screen, &goalPos);
		break;
	    }
	}
    }
     blitCorners(grid, screen,tableSurface);
}

#endif
