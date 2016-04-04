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

#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "const.h"
#include "game.h"

int main(int argc, char *argv[])
{

  	//check if SDL start correctly
	if(SDL_Init(SDL_INIT_VIDEO) == -1)
	  {
	    fprintf(stderr, "SDL initialization error: %s\n", SDL_GetError());
	    exit(EXIT_FAILURE);
	  }
	    fprintf(stderr, "SDL initialization ok\n");

	//Create the window game
 	SDL_Window *window = SDL_CreateWindow(GAME_NAME,
                          SDL_WINDOWPOS_UNDEFINED,
                          SDL_WINDOWPOS_UNDEFINED,
                          W_WIDTH,
			  W_HEIGHT,
                          SDL_WINDOW_MOUSE_FOCUS|SDL_WINDOW_MOUSE_CAPTURE);

    	//set a window icon
	SDL_SetWindowIcon(window, SDL_LoadBMP("img/icon.bmp"));

  	//Create a surface from the window
  	SDL_Surface *screen = NULL;
  	screen = SDL_GetWindowSurface(window);

  	//Check if the window is created*/
	if ( window == NULL || screen == NULL )
   	{
      	    fprintf(stderr, "Creating the main window failed: %s\n", SDL_GetError());
	    exit(EXIT_FAILURE);
    	}
	    fprintf(stderr, "Creating the main window succeed\n");

	//create a map
  	int map[NBR_OF_BLOCKS+1][NBR_OF_BLOCKS+1] = {0}; //+1 to avoid segfault
  	map[1][12] = WOODEN_CASE;
	map[9][9] = WOODEN_CASE;
  	map[5][7] = WOODEN_CASE;
    	map[5][8] = WALL;
      	map[4][9] = WALL;
  	map[5][3] = BALL;
    	map[6][4] = PLAYER;

	//create a ground surface
	SDL_Surface *ground = NULL;
  	SDL_Rect groundPos;
	//load image
	ground = SDL_LoadBMP("img/background.bmp");

	//create a wall surface
  	SDL_Surface *wall = NULL;
  	SDL_Rect wallPos;
	//load image
	wall = SDL_LoadBMP("img/wall.bmp");

	//create a wooden case surface
  	SDL_Surface *woodenCase = NULL;
  	SDL_Rect woodenCasePos;
	//load image
	woodenCase = SDL_LoadBMP("img/box.bmp");

  	//create a ball
  	SDL_Surface *ball = NULL;
  	SDL_Rect ballPos;
	//load image
	ball = IMG_Load("img/ball.png");

  	//player surface
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
	//a player position :
  		SDL_Rect playerPos;
  		playerPos.x = 40;
    		playerPos.y = 40;



	int i = 0, j = 0;
  	for (i = 0; i < NBR_OF_BLOCKS; i++)
	  {
		for (j = 0; j < NBR_OF_BLOCKS; j++)
		  {
		    switch(map[i][j])
		      {
		      case GROUND :
				groundPos.x = i * BOX_SIZE;
				groundPos.y = j * BOX_SIZE;
				SDL_BlitSurface(ground, NULL, screen, &groundPos);
			break;
		      case WOODEN_CASE :
		      		woodenCasePos.x = i * BOX_SIZE;
				woodenCasePos.y = j * BOX_SIZE;
				SDL_BlitSurface(woodenCase, NULL, screen, &woodenCasePos);
			break;
		      case WALL :
		      		wallPos.x = i * BOX_SIZE;
				wallPos.y = j * BOX_SIZE;
				SDL_BlitSurface(wall, NULL, screen, &wallPos);
			break;
		      case BALL :
				groundPos.x = i * BOX_SIZE;
				groundPos.y = j * BOX_SIZE;
				SDL_BlitSurface(ground, NULL, screen, &groundPos);
		      		ballPos.x = i * BOX_SIZE;
				ballPos.y = j * BOX_SIZE;
				SDL_BlitSurface(ball, NULL, screen, &ballPos);
			break;
		      case PLAYER :
				groundPos.x = i * BOX_SIZE;
				groundPos.y = j * BOX_SIZE;
				SDL_BlitSurface(ground, NULL, screen, &groundPos);
		      		playerPos.x = i * BOX_SIZE;
				playerPos.y = j * BOX_SIZE;
				SDL_BlitSurface(playerFront, NULL, screen, &playerPos);

			break;

			}
		  }
	  }


	//refresh the window
  	SDL_UpdateWindowSurface(window);

	//wait for quit event
  int carryOn = 1;
  SDL_Event event;
  SDL_Surface *nextSurface = NULL;
  nextSurface = IMG_Load("img/front.png");
  int xPlayer = 0;
  int yPlayer = 0;
  while(carryOn)
    {
      SDL_WaitEvent(&event);
      switch(event.type)
	{
	case SDL_QUIT:
	  carryOn = 0;
	  break;
	case SDL_KEYDOWN:
	 //get actual position
	  xPlayer = playerPos.x/BOX_SIZE;
	  yPlayer = playerPos.y/BOX_SIZE;
	//listen keyboard:
	switch(event.key.keysym.sym)
	    {
	    case SDLK_RIGHT:
	      	//test what coming next
		//test if border
		if (xPlayer + 1 > NBR_OF_BLOCKS)
			break;
		//test if border
		if(map[xPlayer+1][yPlayer]== WALL)
			break;
		//test if woodenCase
		if (map[xPlayer+1][yPlayer]== WOODEN_CASE && xPlayer + 2 > NBR_OF_BLOCKS &&  map[xPlayer+2][yPlayer]== WOODEN_CASE|| map[xPlayer+2][yPlayer]== WALL )
			break;
		if (map[xPlayer+1][yPlayer]== WOODEN_CASE && map[xPlayer+2][yPlayer]== GROUND)
		{
	      	//move the wooden Case
			woodenCasePos.x = (xPlayer +2) * BOX_SIZE;
			woodenCasePos.y = yPlayer * BOX_SIZE;
			SDL_BlitSurface(woodenCase, NULL, screen, &woodenCasePos);

			//move the player
		  	movePlayer(xPlayer,yPlayer, RIGHT , screen);
		  	//update new player position
		  	playerPos.x  += BOX_SIZE;

			//update status
			map[xPlayer+2][yPlayer] = WOODEN_CASE;
			map[xPlayer][yPlayer] = GROUND;
		      	map[xPlayer+1][yPlayer] = PLAYER;
			break;
		}
		if (map[xPlayer+1][yPlayer]== GROUND)
		{
					//move the player
		  			movePlayer(xPlayer,yPlayer, RIGHT , screen);
		  		  	//update new player position
		  			playerPos.x  += BOX_SIZE;
				       	//update status
				      	map[xPlayer][yPlayer] = GROUND;
				      	map[xPlayer+1][yPlayer] = PLAYER;
		}

	      break;
	    case SDLK_LEFT:
		//test if border
		if (xPlayer - 1 < 0)
		  break;

		//test what coming next
		switch(map[xPlayer-1][yPlayer])
			{
			 case WALL:
			  break;
			 case WOODEN_CASE:
			      	//test if it's possible to move a Wooden Case
			      	if (map[xPlayer-2][yPlayer] != WALL || map[xPlayer-2][yPlayer] != WOODEN_CASE || xPlayer - 2 > 0)
				{
				  		//move the wooden Case
				  		woodenCasePos.x = (xPlayer -2) * BOX_SIZE;
						woodenCasePos.y = yPlayer * BOX_SIZE;
						SDL_BlitSurface(woodenCase, NULL, screen, &woodenCasePos);

				  		//move the player
						SDL_BlitSurface(ground, NULL, screen, &playerPos);
				  		playerPos.x  -= BOX_SIZE;
				  		SDL_BlitSurface(ground, NULL, screen, &playerPos);
						SDL_BlitSurface(playerLeft, NULL, screen, &playerPos);

				  		//update status
				  		map[xPlayer-2][yPlayer] = WOODEN_CASE;
				  		map[xPlayer][yPlayer] = GROUND;
		      				map[xPlayer-1][yPlayer] = PLAYER;
				}
				break;
			case GROUND:
				if (map[xPlayer-1][yPlayer] != WALL && xPlayer -1 >= 0)
			      	{
					//move the player
					SDL_BlitSurface(ground, NULL, screen, &playerPos);
				      	playerPos.x  -= BOX_SIZE;
				      	SDL_BlitSurface(ground, NULL, screen, &playerPos);
					SDL_BlitSurface(playerLeft, NULL, screen, &playerPos);

				      	//update status
				      	map[xPlayer][yPlayer] = GROUND;
				      	map[xPlayer-1][yPlayer] = PLAYER;
				}
				break;
			}
	      break;
	    case SDLK_UP:
		//test what coming next
		switch(map[xPlayer][yPlayer-1])
			{
			 case WALL:
			  break;
			 case WOODEN_CASE:
			      	//test if it's possible to move a Wooden Case
			      	if (map[xPlayer][yPlayer-1] == WOODEN_CASE && map[xPlayer][yPlayer-2] != WALL && yPlayer - 2 > 0)
				{
				  		//move the wooden Case
				  		woodenCasePos.x = xPlayer * BOX_SIZE;
						woodenCasePos.y = (yPlayer - 2)* BOX_SIZE;
						SDL_BlitSurface(woodenCase, NULL, screen, &woodenCasePos);

				  		//move the player
						SDL_BlitSurface(ground, NULL, screen, &playerPos);
				  		playerPos.y  -= BOX_SIZE;
				  		SDL_BlitSurface(ground, NULL, screen, &playerPos);
						SDL_BlitSurface(playerBack, NULL, screen, &playerPos);

				  		//update status
				  		map[xPlayer][yPlayer-2] = WOODEN_CASE;
				  		map[xPlayer][yPlayer] = GROUND;
		      				map[xPlayer][yPlayer-1] = PLAYER;
				}
				break;
			case GROUND:
				if (map[xPlayer][yPlayer-1] != WALL && yPlayer - 1 >= 0)
			      	{
					//move player
					SDL_BlitSurface(ground, NULL, screen, &playerPos);
				      	playerPos.y  -= BOX_SIZE;
				      	SDL_BlitSurface(ground, NULL, screen, &playerPos);
					SDL_BlitSurface(playerBack, NULL, screen, &playerPos);

				      	//update status
				        map[xPlayer][yPlayer] = GROUND;
				      	map[xPlayer][yPlayer-1] = PLAYER;
				}
				break;
			}
	      break;
	    case SDLK_DOWN:
		//test what coming next
		switch(map[xPlayer][yPlayer+1])
			{
			 case WALL:
			  break;
			 case WOODEN_CASE:
			      	//test if it's possible to move a Wooden Case
			      	if (map[xPlayer][yPlayer+1] == WOODEN_CASE && map[xPlayer][yPlayer+2] != WALL && yPlayer + 2 < NBR_OF_BLOCKS)
				{
				  		//move the wooden Case
				  		woodenCasePos.x = xPlayer * BOX_SIZE;
						woodenCasePos.y = (yPlayer + 2) * BOX_SIZE;
						SDL_BlitSurface(woodenCase, NULL, screen, &woodenCasePos);

				  		//move the player
						SDL_BlitSurface(ground, NULL, screen, &playerPos);
				  		playerPos.y  += BOX_SIZE;
				  		SDL_BlitSurface(ground, NULL, screen, &playerPos);
						SDL_BlitSurface(playerFront, NULL, screen, &playerPos);

						//update status
				  		map[xPlayer][yPlayer+2] = WOODEN_CASE;
				  		map[xPlayer][yPlayer] = GROUND;
		      				map[xPlayer][yPlayer+1] = PLAYER;
				}
				break;
			case GROUND:
				if (map[xPlayer][yPlayer+1] != WALL && yPlayer +1 < NBR_OF_BLOCKS)
			      	{
					//move the player
					SDL_BlitSurface(ground, NULL, screen, &playerPos);
				      	playerPos.y  += BOX_SIZE;
				      	SDL_BlitSurface(ground, NULL, screen, &playerPos);
					SDL_BlitSurface(playerFront, NULL, screen, &playerPos);

				       	//update status
				      	map[xPlayer][yPlayer] = GROUND;
				      	map[xPlayer][yPlayer+1] = PLAYER;
				}
				break;
			}
	      break;
	    case SDL_MOUSEBUTTONUP:

	      break;
	    }

	        SDL_UpdateWindowSurface(window);
	}
    }

  	//clean
	SDL_FreeSurface(wall);
	SDL_FreeSurface(ground);
  	SDL_FreeSurface(woodenCase);
	SDL_DestroyWindow(window);
	SDL_Quit();
  	return EXIT_SUCCESS;
}
