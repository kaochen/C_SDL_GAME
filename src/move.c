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

/*This file controls moves for the player.*/

#ifndef MOVE_C
#define MOVE_C

#include "../inc/move.h"

/* move player and update object status" */
int
movePlayer(int xPlayer, int yPlayer, int direction , Square grid[getMax_X_Blocks ()][getMax_Y_Blocks ()]){

        int xStep1 = xPlayer, xStep2 = xPlayer, yStep1 = yPlayer, yStep2 = yPlayer, image = PLAYER_F;
        int xMax = getX_Blocks (), yMax = getY_Blocks ();
   	  switch (direction)
	         {
	         case UP:
              yStep1 = yPlayer -1;
              yStep2 = yPlayer -2;
              image = PLAYER_B;
              break;
            case DOWN:
              yStep1 = yPlayer + 1;
              yStep2 = yPlayer + 2;
              image = PLAYER_F;
              break;
            case RIGHT:
              xStep1 = xPlayer +1;
              xStep2 = xPlayer +2;
              image = PLAYER_R;
              break;
            case LEFT:
              xStep1 = xPlayer -1;
              xStep2 = xPlayer -2;
              image = PLAYER_L;
              break;
            case STILL:
              break;
            }


		  /* Don't go outside */
		  if ((xStep1 < 0 && xStep1 >= xMax) || (yStep1 < 0 && yStep1 >= yMax ))
            return  0;

		  /* test if wall */
	    if (grid[xStep1][yStep1].mainRole == WALL)
          return  0;


		  /* Don't go outside with a box */
		  if (grid[xStep1][yStep1].mainRole == BOX  && (xStep2 < 0 && xStep2 >= xMax ) && (yStep2 < 0 && yStep2 >= yMax ))
          return  0;


		  /* Do not move a box if it is close to a wall or an other box */
		  if (grid[xStep1][yStep1].mainRole == BOX
		      && grid[xStep2][yStep2].mainRole == BOX
		      && grid[xStep2][yStep2].mainRole == WALL){
          return  0;}

		  /* Move a box only if there is space to do it */
		  if ((grid[xStep1][yStep1].mainRole == BOX
		       && grid[xStep2][yStep2].mainRole == GROUND))
		    {

		      /* clean old position */
		      grid[xPlayer][yPlayer].mainRole = GROUND;
		      grid[xPlayer][yPlayer].playerRole = EMPTY;

		      /* update new position */
		      grid[xStep2][yStep2].mainRole = BOX;
		      grid[xStep1][yStep1].mainRole = PLAYER;
		      grid[xStep1][yStep1].playerRole = image;

		      return  1;}

		  /* move only on grounds and Goals */
		  if (grid[xStep1][yStep1].mainRole == GROUND)
		    {

		      /* clean old position */
		      grid[xPlayer][yPlayer].mainRole = GROUND;
		      grid[xPlayer][yPlayer].playerRole = EMPTY;

		      /* update new position */
		      grid[xStep1][yStep1].mainRole = PLAYER;
		      grid[xStep1][yStep1].playerRole = image;

		      return 1;}
return 0;
}

#endif
