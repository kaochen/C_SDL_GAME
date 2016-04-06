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

void movePlayer(int xPlayer, int yPlayer, int direction , SDL_Surface *screen)
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
  		SDL_BlitSurface(ground, NULL, screen, &playerPos);
	switch(direction)
	    {
	    case UP:
			playerPos.y  -= BOX_SIZE;
			SDL_BlitSurface(ground, NULL, screen, &playerPos);
			SDL_BlitSurface(playerBack, NULL, screen, &playerPos);
			break;
	    case DOWN:
			playerPos.y  += BOX_SIZE;
			SDL_BlitSurface(ground, NULL, screen, &playerPos);
			SDL_BlitSurface(playerFront, NULL, screen, &playerPos);
			break;
	    case RIGHT:
			playerPos.x  += BOX_SIZE;
			SDL_BlitSurface(ground, NULL, screen, &playerPos);
			SDL_BlitSurface(playerRight, NULL, screen, &playerPos);
			break;
	    case LEFT:
			playerPos.x  -= BOX_SIZE;
			SDL_BlitSurface(ground, NULL, screen, &playerPos);
			SDL_BlitSurface(playerLeft, NULL, screen, &playerPos);
			break;
	    case STILL:
	      		SDL_BlitSurface(ground, NULL, screen, &playerPos);
			SDL_BlitSurface(playerFront, NULL, screen, &playerPos);
			break;
	    }
	}

//move a case
void moveCase(int xPlayer, int yPlayer, int direction , SDL_Surface *screen)
	{

	//create a wooden case surface
	  	SDL_Surface *woodenCase = NULL;
	//load image
		woodenCase = SDL_LoadBMP("img/box.bmp");

	//case position
	    	SDL_Rect casePos;
	  	casePos.x = xPlayer * BOX_SIZE;
	  	casePos.y = yPlayer * BOX_SIZE;

	//first clean is place
	switch(direction)
	    {
	    case UP:
			casePos.x = xPlayer * BOX_SIZE;
			casePos.y = (yPlayer -2) * BOX_SIZE;
			SDL_BlitSurface(woodenCase, NULL, screen, &casePos);
			break;
	    case DOWN:
			casePos.x = xPlayer * BOX_SIZE;
			casePos.y = (yPlayer +2) * BOX_SIZE;
			SDL_BlitSurface(woodenCase, NULL, screen, &casePos);
			break;
	    case RIGHT:
			casePos.x = (xPlayer +2) * BOX_SIZE;
			casePos.y = yPlayer * BOX_SIZE;
			SDL_BlitSurface(woodenCase, NULL, screen, &casePos);
			break;
	    case LEFT:
			casePos.x = (xPlayer -2) * BOX_SIZE;
			casePos.y = yPlayer * BOX_SIZE;
			SDL_BlitSurface(woodenCase, NULL, screen, &casePos);
			break;
	    case STILL:
			casePos.x = xPlayer * BOX_SIZE;
			casePos.y = yPlayer * BOX_SIZE;
			SDL_BlitSurface(woodenCase, NULL, screen, &casePos);
			break;
	    }
	}



//pick a random ground image
void randomGround(int x, int y, SDL_Surface *screen)
{
	SDL_Surface *ground = NULL;
	ground = SDL_LoadBMP("img/background.bmp");

  	SDL_Surface *ground2 = NULL;
	ground2 = SDL_LoadBMP("img/background2.bmp");

    	SDL_Surface *ground3 = NULL;
	ground3 = SDL_LoadBMP("img/background3.bmp");

  	SDL_Rect groundPos;
    	groundPos.x = x * BOX_SIZE;
	groundPos.y = y * BOX_SIZE;

	int i = 0, randomNumber = 0;
	randomNumber = random_number(0,100);

	if (randomNumber <= 60)
	  {
		SDL_BlitSurface(ground, NULL, screen, &groundPos);
	  }
  	else if(randomNumber >=90)
  	  {
	  	SDL_BlitSurface(ground2, NULL, screen, &groundPos);
	  }
  	else
    	  {
	    	SDL_BlitSurface(ground3, NULL, screen, &groundPos);
	  }

}


//random number between two numbers
int random_number(int min, int max)
	{
	return rand()%(max-min +1) +min;
	}
#endif
