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

//move the player

void movePlayer(int xPlayer, int yPlayer, int direction ,int xyGround, SDL_Surface *screen)
	{
	//create a ground surface
		SDL_Surface *ground = NULL;
		ground = SDL_LoadBMP("img/background.bmp");
	//front face
		SDL_Surface *playerFront = NULL;
		playerFront = IMG_Load("img/front.png");
	//back face
		SDL_Surface *playerBack = NULL;
		playerBack = IMG_Load("img/back.png");
	 //left face
		SDL_Surface *playerLeft = NULL;
		playerLeft = IMG_Load("img/left.png");
      	//rigth face
		SDL_Surface *playerRight = NULL;
		playerRight = IMG_Load("img/right.png");

	//player position
	    	SDL_Rect playerPos;
	  	playerPos.x = xPlayer * BOX_SIZE;
	  	playerPos.y = yPlayer * BOX_SIZE;

	//first clean is place
	 	blitGround(xPlayer, yPlayer, xyGround,screen);
	//read the direction choice
	switch(direction)
	    {
	    case UP:
			playerPos.y  -= BOX_SIZE;
	 		blitGround((playerPos.x/BOX_SIZE),(playerPos.y/BOX_SIZE) , xyGround,screen);
			SDL_BlitSurface(playerBack, NULL, screen, &playerPos);
			break;
	    case DOWN:
			playerPos.y  += BOX_SIZE;
			blitGround((playerPos.x/BOX_SIZE),(playerPos.y/BOX_SIZE) , xyGround,screen);
			SDL_BlitSurface(playerFront, NULL, screen, &playerPos);
			break;
	    case RIGHT:
			playerPos.x  += BOX_SIZE;
			blitGround((playerPos.x/BOX_SIZE),(playerPos.y/BOX_SIZE) , xyGround,screen);
			SDL_BlitSurface(playerRight, NULL, screen, &playerPos);
			break;
	    case LEFT:
			playerPos.x  -= BOX_SIZE;
			blitGround((playerPos.x/BOX_SIZE),(playerPos.y/BOX_SIZE) , xyGround,screen);
			SDL_BlitSurface(playerLeft, NULL, screen, &playerPos);
			break;
	    case STILL:
	 		blitGround((playerPos.x/BOX_SIZE),(playerPos.y/BOX_SIZE) , xyGround,screen);
			SDL_BlitSurface(playerFront, NULL, screen, &playerPos);

			break;
	    }
	      	//clean
	  	SDL_FreeSurface(playerFront);
	    	SDL_FreeSurface(playerBack);
	      	SDL_FreeSurface(playerLeft);
	  	SDL_FreeSurface(playerRight);
	  	SDL_FreeSurface(ground);
	}

//move a box
void moveBox(int xPlayer, int yPlayer, int map[][NBR_OF_BLOCKS], int direction , SDL_Surface *screen)
	{

	//create a box surface
	SDL_Surface *box = NULL;
	//load image
	box = SDL_LoadBMP("img/box.bmp");

	//goal
 	SDL_Surface *goal = NULL;
	//load image
	goal = IMG_Load("img/ball.png");

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
	//blit if box on a goal
	if (map[boxPos.x/BOX_SIZE][boxPos.y/BOX_SIZE] == BALL || map[boxPos.x/BOX_SIZE][boxPos.y/BOX_SIZE] == BOX_OK)
	    		{
			SDL_BlitSurface(box, NULL, screen, &boxPos);
			SDL_BlitSurface(goal, NULL, screen, &boxPos);
			}

	 else
		  	{
			SDL_BlitSurface(box, NULL, screen, &boxPos);
			}
	//clean
	SDL_FreeSurface(box);
	SDL_FreeSurface(goal);
}



//pick a random ground image

//blit ground
void blitGround(int x, int y, int typeOfGround,SDL_Surface *screen)
	{
	SDL_Surface *ground1 = NULL;
	ground1 = SDL_LoadBMP("img/background.bmp");

  	SDL_Surface *ground2 = NULL;
	ground2 = SDL_LoadBMP("img/background2.bmp");

    	SDL_Surface *ground3 = NULL;
	ground3 = SDL_LoadBMP("img/background3.bmp");

  	SDL_Rect groundPos;
    	groundPos.x = x * BOX_SIZE;
	groundPos.y = y * BOX_SIZE;

	switch(typeOfGround)
	  {
	  case GROUND1:
		SDL_BlitSurface(ground1, NULL, screen, &groundPos);
	    break;
	  case GROUND2:
		SDL_BlitSurface(ground2, NULL, screen, &groundPos);
	    break;
	  case GROUND3:
		SDL_BlitSurface(ground3, NULL, screen, &groundPos);
	    break;
	  }
SDL_FreeSurface(ground1);
SDL_FreeSurface(ground2);
SDL_FreeSurface(ground3);
}

//blitWalls
void blitWalls(int x, int y, int map[][NBR_OF_BLOCKS],SDL_Surface *screen)
{
	//create a wall 1 surface
  	SDL_Surface *wall1 = NULL;
	wall1 = IMG_Load("img/wall.png");

  	//create a wall 2 surface
  	SDL_Surface *wall2 = NULL;
	wall2 = IMG_Load("img/wall2.png");

  	//create a wall 3 surface
  	SDL_Surface *wall3 = NULL;
	wall3 = IMG_Load("img/wall3.png");

    	//create a wall 4 surface
  	SDL_Surface *wall4 = NULL;
	wall4 = IMG_Load("img/wall4.png");

	//set position for wall blocks
    	SDL_Rect wallPos;

	//create the left wall border
  	SDL_Surface *wallBorderLeft = NULL;
	wallBorderLeft = IMG_Load("img/wallBorderLeft.png");

  	//create the right wall border
  	SDL_Surface *wallBorderRight = NULL;
	wallBorderRight = IMG_Load("img/wallBorderRight.png");

  	//create the top wall border
  	SDL_Surface *wallBorderTop = NULL;
	wallBorderTop = IMG_Load("img/wallBorderTop.png");

  	//create the bottom wall border
  	SDL_Surface *wallBorderBottom = NULL;
	wallBorderBottom = IMG_Load("img/wallBorderBottom.png");

	wallPos.x = x * BOX_SIZE;
	wallPos.y = y * BOX_SIZE;

  	//blit a random Wall Surface
  	int i = 0, randomNumber = 0;
	randomNumber = random_number(0,100);
	if (randomNumber < 40)
		{
			SDL_BlitSurface(wall1, NULL, screen, &wallPos);
		}
  	else if(randomNumber < 60)
    		{
			SDL_BlitSurface(wall2, NULL, screen, &wallPos);
		}
    	else if(randomNumber < 80)
    		{
			SDL_BlitSurface(wall3, NULL, screen, &wallPos);
		}
  	else
    		{
		    	SDL_BlitSurface(wall4, NULL, screen, &wallPos);
		}


  //blit wall border expect on window sides
   	//blit left border
  	if (x != 0 && map[x-1][y] != WALL )
    		{
	  	SDL_BlitSurface(wallBorderLeft, NULL, screen, &wallPos);
		}
    	//blit right border
  	if (x != NBR_OF_BLOCKS && map[x+1][y] != WALL)
    		{
	  	SDL_BlitSurface(wallBorderRight, NULL, screen, &wallPos);
		}
    	//blit top border
  	if ( y!= 0 && map[x][y-1] != WALL)
    		{
	  	SDL_BlitSurface(wallBorderTop, NULL, screen, &wallPos);
		}
    	//blit bottom border
  	if (y != NBR_OF_BLOCKS && map[x][y+1] != WALL)
    		{
	  	SDL_BlitSurface(wallBorderBottom, NULL, screen, &wallPos);
		}
	//clean
  	SDL_FreeSurface(wall1);
    	SDL_FreeSurface(wall2);
      	SDL_FreeSurface(wall3);
        SDL_FreeSurface(wall4);
  	SDL_FreeSurface(wallBorderTop);
  	SDL_FreeSurface(wallBorderBottom);
  	SDL_FreeSurface(wallBorderLeft);
  	SDL_FreeSurface(wallBorderRight);
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
