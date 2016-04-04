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

#include "const.h"

//move the player

void movePlayer(int xPlayer, int yPlayer, int direction , SDL_Surface *screen)
	{
	//create a ground surface
		SDL_Surface *ground = NULL;
  		SDL_Rect groundPos;
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
	//update status
	/*	map[xPlayer+2][yPlayer] = WOODEN_CASE;
		//map[xPlayer][yPlayer] = GROUND;
		map[xPlayer+1][yPlayer] = PLAYER;*/

	}










#endif
