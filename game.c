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

#include "const.h"
#include "game.h"
#include "sprites.h"

//move the player and blit player image
void blitPlayer(int xPlayer, int yPlayer, int direction ,int xyGround, SDL_Surface *screen, Sprites tableSurface[NBR_OF_IMAGES])
	{
	//player position
	    	SDL_Rect playerPos;
	  	playerPos.x = xPlayer * BOX_SIZE;
	  	playerPos.y = yPlayer * BOX_SIZE;

	//first clean is place
	 	blitGround(xPlayer, yPlayer, xyGround,screen,tableSurface);
	//read the direction choice
	switch(direction)
	    {
	    case UP:
			playerPos.y  -= BOX_SIZE;
	 		blitGround((playerPos.x/BOX_SIZE),(playerPos.y/BOX_SIZE) , xyGround,screen,tableSurface);
	      	      	SDL_BlitSurface(tableSurface[PLAYER_BACK].image, NULL, screen, &playerPos);
			break;
	    case DOWN:
			playerPos.y  += BOX_SIZE;
			blitGround((playerPos.x/BOX_SIZE),(playerPos.y/BOX_SIZE) , xyGround,screen,tableSurface);
	      		SDL_BlitSurface(tableSurface[PLAYER_FRONT].image, NULL, screen, &playerPos);
			break;
	    case RIGHT:
			playerPos.x  += BOX_SIZE;
			blitGround((playerPos.x/BOX_SIZE),(playerPos.y/BOX_SIZE) , xyGround,screen,tableSurface);
	      	      	SDL_BlitSurface(tableSurface[PLAYER_RIGHT].image, NULL, screen, &playerPos);
			break;
	    case LEFT:
			playerPos.x  -= BOX_SIZE;
			blitGround((playerPos.x/BOX_SIZE),(playerPos.y/BOX_SIZE) , xyGround,screen,tableSurface);
	      	      	SDL_BlitSurface(tableSurface[PLAYER_LEFT].image, NULL, screen, &playerPos);
			break;
	    case STILL:
	 		blitGround((playerPos.x/BOX_SIZE),(playerPos.y/BOX_SIZE) , xyGround,screen,tableSurface);
	      	      	SDL_BlitSurface(tableSurface[PLAYER_FRONT].image, NULL, screen, &playerPos);
			break;
	    }
	}



//move a box
void moveBox(int xPlayer, int yPlayer, int map[][NBR_OF_BLOCKS], int direction , SDL_Surface *screen, Sprites tableSurface[NBR_OF_IMAGES])
	{
	//box position
	    	SDL_Rect boxPos;
	  	boxPos.x = xPlayer * BOX_SIZE;
	  	boxPos.y = yPlayer * BOX_SIZE;

	switch(direction)
	    {
	    case UP:
			boxPos.x = xPlayer * BOX_SIZE;
			boxPos.y = (yPlayer -2) * BOX_SIZE;
			break;
	    case DOWN:
			boxPos.x = xPlayer * BOX_SIZE;
			boxPos.y = (yPlayer +2) * BOX_SIZE;
			break;
	    case RIGHT:
			boxPos.x = (xPlayer +2) * BOX_SIZE;
			boxPos.y = yPlayer * BOX_SIZE;
			break;
	    case LEFT:
			boxPos.x = (xPlayer -2) * BOX_SIZE;
			boxPos.y = yPlayer * BOX_SIZE;
			break;
	    case STILL:
			boxPos.x = xPlayer * BOX_SIZE;
			boxPos.y = yPlayer * BOX_SIZE;
			break;
	    }
	SDL_BlitSurface(tableSurface[GROUND1_IMAGE].image, NULL, screen, &boxPos);
	//blit if box on a goal
	if (map[boxPos.x/BOX_SIZE][boxPos.y/BOX_SIZE] == GOAL || map[boxPos.x/BOX_SIZE][boxPos.y/BOX_SIZE] == BOX_OK)
	    		{
			SDL_BlitSurface(tableSurface[BOX_IMAGE_OK].image, NULL, screen, &boxPos);
			}

	 else
		  	{
			SDL_BlitSurface(tableSurface[BOX_IMAGE].image, NULL, screen, &boxPos);
			}
}



//pick a random ground image

//blit ground
void blitGround(int x, int y, int typeOfGround,SDL_Surface *screen, Sprites tableSurface[NBR_OF_IMAGES])
	{
  	SDL_Rect groundPos;
    	groundPos.x = x * BOX_SIZE;
	groundPos.y = y * BOX_SIZE;

	switch(typeOfGround)
	  {
	  case GROUND1:
		SDL_BlitSurface(tableSurface[GROUND1_IMAGE].image, NULL, screen, &groundPos);
	    break;
	  case GROUND2:
		SDL_BlitSurface(tableSurface[GROUND2_IMAGE].image, NULL, screen, &groundPos);
	    break;
	  case GROUND3:
		SDL_BlitSurface(tableSurface[GROUND3_IMAGE].image, NULL, screen, &groundPos);
	    break;
	  }
}

//blitWalls
void blitWalls(int x, int y, int map[][NBR_OF_BLOCKS],SDL_Surface *screen, Sprites tableSurface[NBR_OF_IMAGES])
{

	//set position for wall blocks
    	SDL_Rect wallPos;
	wallPos.x = x * BOX_SIZE;
	wallPos.y = y * BOX_SIZE;

  	//blit a random Wall Surface
  	int i = 0, randomNumber = 0;
	randomNumber = random_number(0,100);
	if (randomNumber < 40)
		{
			SDL_BlitSurface(tableSurface[WALL1_IMAGE].image, NULL, screen, &wallPos);
		}
  	else if(randomNumber < 60)
    		{
			SDL_BlitSurface(tableSurface[WALL2_IMAGE].image, NULL, screen, &wallPos);
		}
    	else if(randomNumber < 80)
    		{
			SDL_BlitSurface(tableSurface[WALL3_IMAGE].image, NULL, screen, &wallPos);
		}
  	else
    		{
		    	SDL_BlitSurface(tableSurface[WALL4_IMAGE].image, NULL, screen, &wallPos);
		}


  //blit wall border expect on window sides
   	//blit left border
  	if (x != 0 && map[x-1][y] != WALL )
    		{
	  	SDL_BlitSurface(tableSurface[WALL_LEFT].image, NULL, screen, &wallPos);
		}
    	//blit right border
  	if (x != NBR_OF_BLOCKS && map[x+1][y] != WALL)
    		{
	  	SDL_BlitSurface(tableSurface[WALL_RIGHT].image, NULL, screen, &wallPos);
		}
    	//blit top border
  	if ( y!= 0 && map[x][y-1] != WALL)
    		{
	  	SDL_BlitSurface(tableSurface[WALL_TOP].image, NULL, screen, &wallPos);
		}
    	//blit bottom border
  	if (y != NBR_OF_BLOCKS && map[x][y+1] != WALL)
    		{
	  	SDL_BlitSurface(tableSurface[WALL_BOTTOM].image, NULL, screen, &wallPos);
		}
}

//add a more ground choice
int addRandomGround()
	{
		int i = 0, randomNumber = 0;
		randomNumber = random_number(0,100);
		  if (randomNumber <= 60)
			  {
				return GROUND1;
			  }
		  	else if(randomNumber >=90)
		  	  {
				return GROUND2;
			  }
		  	else
		    	  {
				return GROUND3;
			    }
	}


//random number between two numbers
int random_number(int min, int max)
	{
	return rand()%(max-min +1) +min;
	}
#endif
