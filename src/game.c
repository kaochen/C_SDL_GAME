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

#ifndef GAME_C
#define GAME_C

#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>

#include "../inc/const.h"
#include "../inc/game.h"
#include "../inc/sprites.h"

/* move the player and blit player images */
void blitPlayer(int xPlayer, int yPlayer, int direction,
		Square grid[][MAX_BLOCKS], SDL_Surface * screen,
		Sprites tableSurface[NBR_OF_IMAGES])
{
    /* set player position */
    SDL_Rect playerPos;
    playerPos.x = xPlayer * BOX_SIZE;
    playerPos.y = yPlayer * BOX_SIZE;

    /* first clean is place */
    blitGround(xPlayer, yPlayer, grid, screen, tableSurface);
    /* read the direction choice */
    switch (direction) {
    case UP:
	playerPos.y -= BOX_SIZE;
	blitGround((playerPos.x / BOX_SIZE), (playerPos.y / BOX_SIZE),
		   grid, screen, tableSurface);
	SDL_BlitSurface(tableSurface[PLAYER_BACK].image, NULL, screen,
			&playerPos);
	break;
    case DOWN:
	playerPos.y += BOX_SIZE;
	blitGround((playerPos.x / BOX_SIZE), (playerPos.y / BOX_SIZE),
		   grid, screen, tableSurface);
	SDL_BlitSurface(tableSurface[PLAYER_FRONT].image, NULL, screen,
			&playerPos);
	break;
    case RIGHT:
	playerPos.x += BOX_SIZE;
	blitGround((playerPos.x / BOX_SIZE), (playerPos.y / BOX_SIZE),
		   grid, screen, tableSurface);
	SDL_BlitSurface(tableSurface[PLAYER_RIGHT].image, NULL, screen,
			&playerPos);
	break;
    case LEFT:
	playerPos.x -= BOX_SIZE;
	blitGround((playerPos.x / BOX_SIZE), (playerPos.y / BOX_SIZE),
		   grid, screen, tableSurface);
	SDL_BlitSurface(tableSurface[PLAYER_LEFT].image, NULL, screen,
			&playerPos);
	break;
    case STILL:
	blitGround((playerPos.x / BOX_SIZE), (playerPos.y / BOX_SIZE),
		   grid, screen, tableSurface);
	SDL_BlitSurface(tableSurface[PLAYER_FRONT].image, NULL, screen,
			&playerPos);
	break;
    }
}

/* move a box */
void moveBox(int xPlayer, int yPlayer, Square grid[][MAX_BLOCKS],
	     int direction, SDL_Surface * screen,
	     Sprites tableSurface[NBR_OF_IMAGES])
{
    /* set box position */
    SDL_Rect boxPos;
    boxPos.x = xPlayer * BOX_SIZE;
    boxPos.y = yPlayer * BOX_SIZE;

    switch (direction) {
    case UP:
	boxPos.x = xPlayer * BOX_SIZE;
	boxPos.y = (yPlayer - 2) * BOX_SIZE;
	break;
    case DOWN:
	boxPos.x = xPlayer * BOX_SIZE;
	boxPos.y = (yPlayer + 2) * BOX_SIZE;
	break;
    case RIGHT:
	boxPos.x = (xPlayer + 2) * BOX_SIZE;
	boxPos.y = yPlayer * BOX_SIZE;
	break;
    case LEFT:
	boxPos.x = (xPlayer - 2) * BOX_SIZE;
	boxPos.y = yPlayer * BOX_SIZE;
	break;
    case STILL:
	boxPos.x = xPlayer * BOX_SIZE;
	boxPos.y = yPlayer * BOX_SIZE;
	break;
    }
    SDL_BlitSurface(tableSurface[GROUND1_IMAGE].image, NULL, screen,
		    &boxPos);
    /* blit BOX_IMAGE_OK if Box on a goal */
    if (grid[boxPos.x / BOX_SIZE][boxPos.y / BOX_SIZE].mainRole == GOAL
	|| grid[boxPos.x / BOX_SIZE][boxPos.y / BOX_SIZE].objectType ==
	GOAL) {
	SDL_BlitSurface(tableSurface[BOX_IMAGE_OK].image, NULL, screen,
			&boxPos);
    }

    else {
	SDL_BlitSurface(tableSurface[BOX_IMAGE].image, NULL, screen,
			&boxPos);
    }
}

/* blit ground */
void blitGround(int x, int y, Square grid[][MAX_BLOCKS],
		SDL_Surface * screen, Sprites tableSurface[NBR_OF_IMAGES])
{
    SDL_Rect groundPos;
    groundPos.x = x * BOX_SIZE;
    groundPos.y = y * BOX_SIZE;

    switch (grid[x][y].subRole) {
    case GROUND1:
	SDL_BlitSurface(tableSurface[GROUND1_IMAGE].image, NULL, screen,
			&groundPos);
	break;
    case GROUND2:
	SDL_BlitSurface(tableSurface[GROUND2_IMAGE].image, NULL, screen,
			&groundPos);
	break;
    case GROUND3:
	SDL_BlitSurface(tableSurface[GROUND3_IMAGE].image, NULL, screen,
			&groundPos);
	break;
    }
    /* Blit Goals if needed */
    if (grid[x][y].objectType == GOAL)
	SDL_BlitSurface(tableSurface[GOAL_IMAGE].image, NULL, screen,
			&groundPos);

}

/* blit Walls */
void blitWalls(int x, int y, Square grid[][MAX_BLOCKS],
	       SDL_Surface * screen, Sprites tableSurface[NBR_OF_IMAGES])
{

    /* set position for wall blocks */
    SDL_Rect wallPos;
    wallPos.x = x * BOX_SIZE;
    wallPos.y = y * BOX_SIZE;

    /* blit a random Wall Surface */
    int randomNumber = 0;
    randomNumber = random_number(0, 100);
    if (randomNumber < 40) {
	SDL_BlitSurface(tableSurface[WALL1_IMAGE].image, NULL, screen,
			&wallPos);
    } else if (randomNumber < 60) {
	SDL_BlitSurface(tableSurface[WALL2_IMAGE].image, NULL, screen,
			&wallPos);
    } else if (randomNumber < 80) {
	SDL_BlitSurface(tableSurface[WALL3_IMAGE].image, NULL, screen,
			&wallPos);
    } else {
	SDL_BlitSurface(tableSurface[WALL4_IMAGE].image, NULL, screen,
			&wallPos);
    }

    /* blit wall border, expect on window sides */
    /* blit left border */
    if (x != 0 && grid[x - 1][y].mainRole != WALL && grid[x - 1][y].mainRole != TOP_LEFT && grid[x - 1][y].mainRole != BOTTOM_LEFT) {
	SDL_BlitSurface(tableSurface[WALL_LEFT].image, NULL, screen,
			&wallPos);
    }
    /* blit right border */
    if (x != X_BLOCKS && grid[x + 1][y].mainRole != WALL && grid[x + 1][y].mainRole != TOP_RIGHT && grid[x + 1][y].mainRole != BOTTOM_RIGHT) {
	SDL_BlitSurface(tableSurface[WALL_RIGHT].image, NULL, screen,
			&wallPos);
    }
    /* blit top border */
    if (y != 0 && grid[x][y - 1].mainRole != WALL && grid[x][y - 1].mainRole != TOP_RIGHT && grid[x][y - 1].mainRole != TOP_LEFT) {
	SDL_BlitSurface(tableSurface[WALL_TOP].image, NULL, screen,
			&wallPos);
    }
    /* blit bottom border */
    if (y != Y_BLOCKS && grid[x][y + 1].mainRole != WALL && grid[x][y + 1].mainRole != BOTTOM_RIGHT && grid[x][y + 1].mainRole != BOTTOM_LEFT) {
	SDL_BlitSurface(tableSurface[WALL_BOTTOM].image, NULL, screen,
			&wallPos);
    }
}

/* add a more ground choice */
int randomGround(Square grid[][MAX_BLOCKS])
{
    int x = 0, y = 0, randomNumber = 0;
    for (y = 0; y < Y_BLOCKS; y++) {
        for (x = 1; x < Y_BLOCKS; x++) {

             randomNumber = random_number(0, 100);
             if (randomNumber <= 60) {
	         grid[x][y].subRole = GROUND1;
             } else if (randomNumber >= 90) {
	         grid[x][y].subRole = GROUND2;
             } else {
	         grid[x][y].subRole = GROUND3;
             }

        }
    }
    return EXIT_SUCCESS;
}

/*change OUTSIDE subRole to get more choice between OUTSIDE, OUTSIDE2...*/
int randomOutside(Square grid[][MAX_BLOCKS])
{
    int x = 0, y = 0, randomNumber = 0;
    for (y = 0; y < Y_BLOCKS; y++) {
        for (x = 0; x < X_BLOCKS; x++) {
          if(grid[x][y].mainRole == OUTSIDE) {
                randomNumber = random_number(0, 100);
                if (randomNumber <= 87) {
	            grid[x][y].subRole = OUTSIDE;
                } else if (randomNumber >= 90) {
	            grid[x][y].subRole = OUTSIDE2;
                     fprintf(stderr, "add OUTSIDE2 %d/%d\n", x,y );
                } else {
	            grid[x][y].subRole = OUTSIDE3;
                }
            }
        }
    }
  return EXIT_SUCCESS;
}

/* random number between two numbers */
int random_number(int min, int max)
{
    return rand() % (max - min + 1) + min;
}


/* blit corners */
int blitCorners(Square grid[][MAX_BLOCKS], SDL_Surface * screen, Sprites tableSurface[NBR_OF_IMAGES]){
    SDL_Rect pos;
    int x = 1, y = 1;
    for (y = 1; y < Y_BLOCKS; y++){
        for (x = 1; x < X_BLOCKS; x++){
            pos.x = x*BOX_SIZE;
            pos.y = y*BOX_SIZE;
            /*blit top right corners */
            if (grid[x][y].mainRole == WALL && grid[x][y-1].mainRole == OUTSIDE && grid[x+1][y-1].mainRole == OUTSIDE && grid[x+1][y].mainRole == OUTSIDE )
            {
             SDL_BlitSurface(tableSurface[CORNER_TR].image, NULL,	screen, &pos);
             fprintf(stderr, "found TOP_RIGHT %d/%d\n", x,y );
            }
            /*blit top left corners */
            if (grid[x][y].mainRole == WALL && grid[x][y-1].mainRole == OUTSIDE && grid[x-1][y-1].mainRole == OUTSIDE && grid[x-1][y].mainRole == OUTSIDE )
            {
		      SDL_BlitSurface(tableSurface[CORNER_TL].image, NULL,	screen, &pos);
            fprintf(stderr, "found TOP_LEFT %d/%d\n", x,y );
            }
            /*blit bottom right corners */
            if (grid[x][y].mainRole == WALL && grid[x][y+1].mainRole == OUTSIDE && grid[x+1][y+1].mainRole == OUTSIDE && grid[x+1][y].mainRole == OUTSIDE )
            {
              SDL_BlitSurface(tableSurface[CORNER_BR].image, NULL,screen, &pos);
              fprintf(stderr, "found BOTTOM_RIGHT %d/%d\n", x,y );
            }
            if (grid[x][y].mainRole == WALL && grid[x][y+1].mainRole == OUTSIDE && grid[x-1][y+1].mainRole == OUTSIDE && grid[x-1][y].mainRole == OUTSIDE )
            {
		    SDL_BlitSurface(tableSurface[CORNER_BL].image, NULL,	screen, &pos);
            fprintf(stderr, "found BOTTOM_LEFT %d/%d\n", x,y );
            }
        }
   }
   return EXIT_SUCCESS;
}
#endif
