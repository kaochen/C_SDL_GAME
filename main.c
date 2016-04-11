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
#include <time.h>

#include "const.h"
#include "game.h"
#include "level.h"
#include "menu.h"

int main(int argc, char *argv[])
{
  	 srand(time(NULL)); //start random processeur just once
			    //
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

	//create a map with coordinates x,y to locate things
  	int map[NBR_OF_BLOCKS][NBR_OF_BLOCKS] = {0};

	//how many level are in the file
	int max_Levels = nbr_of_level();
  	int levelChoice = 0;
  	//levelChoice = levelSelector(window, screen);

  	//load the level from the levels.txt
	//readlevel(map, levelChoice);
	readlevel(map, 0);

	//create a ground map to store ground type from the randomGround function
	int mapGround[NBR_OF_BLOCKS][NBR_OF_BLOCKS] = {0};


	//a player position :
  		SDL_Rect playerPos;
    		int xPlayer = 0;
 		int yPlayer = 0;

	//display the level using map and mapGround
	displayLevel(map,mapGround, screen);

	//refresh the window
  	SDL_UpdateWindowSurface(window);



  //get the player position
  int x = 0, y = 0;
  	for (x = 0; x < NBR_OF_BLOCKS; x++)
	  {
		for (y = 0; y < NBR_OF_BLOCKS; y++)
		  {
		    if (map[x][y] == PLAYER)
		      {
			playerPos.x = x * BOX_SIZE;
			playerPos.y = y * BOX_SIZE;
		      }
	  	   }
	}

//wait for quit event
  int carryOn = 1 ;
  SDL_Event event;
  while(carryOn)
    {
      SDL_WaitEvent(&event);
      switch(event.type)
	{
	case SDL_QUIT:
	  carryOn = 0;
	  break;
	case SDL_KEYDOWN:
	 //get actual position in the map reference
	  xPlayer = playerPos.x/BOX_SIZE;
	  yPlayer = playerPos.y/BOX_SIZE;
	//listen keyboard:
	switch(event.key.keysym.sym)
	    {
	    case SDLK_RIGHT:
		//Don't go outside
		if (xPlayer + 1 >= NBR_OF_BLOCKS)
			break;
		//test if wall
		if(map[xPlayer+1][yPlayer]== WALL || map[xPlayer+1][yPlayer]== BOX_OK)
			break;
	      	//Don't go outside with a case
	      	if (map[xPlayer+1][yPlayer]== WOODEN_CASE && xPlayer + 2 >= NBR_OF_BLOCKS)
		      break;
		//Do not move a case if it is close to a wall or an other case
		if (map[xPlayer+1][yPlayer]== WOODEN_CASE && map[xPlayer+2][yPlayer]== WOODEN_CASE && map[xPlayer+2][yPlayer]== WALL)
			break;
	      	//Move a case only if there is space to do it
		if (map[xPlayer+1][yPlayer]== WOODEN_CASE && map[xPlayer+2][yPlayer]== GROUND || map[xPlayer+1][yPlayer]== WOODEN_CASE && map[xPlayer+2][yPlayer] == BALL)
		{
	      	//move the box
			moveBox(xPlayer,yPlayer,map, RIGHT,screen);
		//move the player
		  	movePlayer(xPlayer,yPlayer, RIGHT , mapGround[xPlayer][yPlayer], screen);
		//update new player position
		  	playerPos.x  += BOX_SIZE;
		//update status
		if(map[xPlayer+2][yPlayer] == BALL)
		    {
		      	map[xPlayer+2][yPlayer] = BOX_OK;
		    }
		 else
		   {
		     map[xPlayer+2][yPlayer] = WOODEN_CASE;
		   }
			map[xPlayer][yPlayer] = GROUND;
		      	map[xPlayer+1][yPlayer] = PLAYER;
			break;
		}
	      	//move only on ground
		if (map[xPlayer+1][yPlayer]== GROUND)
		{
					//move the player
		  		  	movePlayer(xPlayer,yPlayer, RIGHT , mapGround[xPlayer][yPlayer], screen);
		  		  	//update new player position
		  			playerPos.x  += BOX_SIZE;
				       	//update status
				      	map[xPlayer][yPlayer] = GROUND;
				      	map[xPlayer+1][yPlayer] = PLAYER;
		  			break;
		}
	      break;


	    case SDLK_LEFT:
	      //Don't go outside
		if (xPlayer - 1 < 0)
			break;
		//test if wall
		if(map[xPlayer-1][yPlayer]== WALL || map[xPlayer-1][yPlayer]== BOX_OK)
			break;
	      	//Don't go outside with a case
	      	if (map[xPlayer-1][yPlayer]== WOODEN_CASE && xPlayer - 2 < 0)
		      break;
		//Do not move a case if it is close to a wall or an other case
		if (map[xPlayer-1][yPlayer]== WOODEN_CASE && map[xPlayer-2][yPlayer]== WOODEN_CASE && map[xPlayer-2][yPlayer]== WALL)
			break;
	      	//Move a case only if there is space to do it
		if (map[xPlayer-1][yPlayer]== WOODEN_CASE && map[xPlayer-2][yPlayer]== GROUND || map[xPlayer-1][yPlayer]== WOODEN_CASE && map[xPlayer-2][yPlayer] == BALL)
		{
	      	//move the box
			moveBox(xPlayer,yPlayer,map, LEFT,screen);
		//move the player
		  	movePlayer(xPlayer,yPlayer, LEFT , mapGround[xPlayer][yPlayer], screen);
		//update new player position
		  	playerPos.x  -= BOX_SIZE;
		//update status
		 if(map[xPlayer-2][yPlayer] == BALL)
		    {
		      	map[xPlayer-2][yPlayer] = BOX_OK;
		    }
		 else
		   {
		     map[xPlayer-2][yPlayer] = WOODEN_CASE;
		   }
		        map[xPlayer-2][yPlayer] = WOODEN_CASE;
			map[xPlayer][yPlayer] = GROUND;
		      	map[xPlayer-1][yPlayer] = PLAYER;
			break;
		}
	      	//move only on ground
		if (map[xPlayer-1][yPlayer]== GROUND)
		{
					//move the player
		  		  	movePlayer(xPlayer,yPlayer, LEFT , mapGround[xPlayer][yPlayer], screen);
		  		  	//update new player position
		  			playerPos.x  -= BOX_SIZE;
				       	//update status
				      	map[xPlayer][yPlayer] = GROUND;
				      	map[xPlayer-1][yPlayer] = PLAYER;
		  			break;
		}
	      break;


	    case SDLK_UP:
	      //Don't go outside
		if (yPlayer - 1 < 0 )
			break;
		//test if wall
		if(map[xPlayer][yPlayer-1]== WALL || map[xPlayer][yPlayer-1]== BOX_OK)
			break;
	      	//Don't go outside with a case
	      	if (map[xPlayer][yPlayer-1]== WOODEN_CASE && yPlayer - 2 < 0)
		      break;
		//Do not move a case if it is close to a wall or an other case
		if (map[xPlayer][yPlayer-1]== WOODEN_CASE && map[xPlayer][yPlayer-2]== WOODEN_CASE && map[xPlayer][yPlayer-2]== WALL)
			break;
	      	//Move a case only if there is space to do it
		if (map[xPlayer][yPlayer-1]== WOODEN_CASE && map[xPlayer][yPlayer-2]== GROUND || map[xPlayer][yPlayer-1]== WOODEN_CASE && map[xPlayer][yPlayer-2] == BALL)
		{
	      	//move the box
			moveBox(xPlayer,yPlayer,map,UP,screen);
		//move the player
		  	movePlayer(xPlayer,yPlayer, UP , mapGround[xPlayer][yPlayer], screen);
		//update new player position
		  	playerPos.y  -= BOX_SIZE;
		//update status
		if(map[xPlayer][yPlayer-2] == BALL)
		    {
		      	map[xPlayer][yPlayer-2] = BOX_OK;
		    }
		 else
		   {
		     map[xPlayer][yPlayer-2] = WOODEN_CASE;
		   }
			map[xPlayer][yPlayer] = GROUND;
		      	map[xPlayer][yPlayer-1] = PLAYER;
			break;
		}
	      	//move only on ground
		if (map[xPlayer][yPlayer-1]== GROUND)
		{
					//move the player
		  		  	movePlayer(xPlayer,yPlayer, UP , mapGround[xPlayer][yPlayer], screen);
		  		  	//update new player position
		  			playerPos.y  -= BOX_SIZE;
				       	//update status
				      	map[xPlayer][yPlayer] = GROUND;
				      	map[xPlayer][yPlayer-1] = PLAYER;
		  			break;
		}
	      break;


	    case SDLK_DOWN:
			      //Don't go outside
		if (yPlayer + 1 >= NBR_OF_BLOCKS)
			break;
		//test if wall
		if(map[xPlayer][yPlayer+1]== WALL)
			break;
		if(map[xPlayer][yPlayer+1]== BOX_OK)
		  	break;
	      	//Don't go outside with a case
	      	if (map[xPlayer][yPlayer+1]== WOODEN_CASE && yPlayer + 2 >= NBR_OF_BLOCKS)
		      break;
		//Do not move a case if it is close to a wall or an other case
		if (map[xPlayer][yPlayer+1]== WOODEN_CASE && map[xPlayer][yPlayer+2]== WOODEN_CASE && map[xPlayer][yPlayer+2]== WALL)
			break;
	      	//Move a case only if there is space to do it
		if (map[xPlayer][yPlayer+1]== WOODEN_CASE && map[xPlayer][yPlayer+2]== GROUND || map[xPlayer][yPlayer+1]== WOODEN_CASE && map[xPlayer][yPlayer+2] == BALL)
		{
	      	//move the Box
			moveBox(xPlayer,yPlayer,map, DOWN,screen);
		//move the player
		  	movePlayer(xPlayer,yPlayer, DOWN , mapGround[xPlayer][yPlayer], screen);
		//update new player position
		  	playerPos.y  += BOX_SIZE;
		//update status
		if(map[xPlayer][yPlayer+2] == BALL)
		    {
		      	map[xPlayer][yPlayer+2] = BOX_OK;
		    }
		 else
		   {
		     map[xPlayer][yPlayer+2] = WOODEN_CASE;
		   }
			map[xPlayer][yPlayer] = GROUND;
		      	map[xPlayer][yPlayer+1] = PLAYER;
			break;
		}
	      	//move only on ground
		if (map[xPlayer][yPlayer+1]== GROUND)
		{
					//move the player
		  			movePlayer(xPlayer,yPlayer, DOWN , mapGround[xPlayer][yPlayer], screen);
		  		  	//update new player position
		  			playerPos.y  += BOX_SIZE;
				       	//update status
				      	map[xPlayer][yPlayer] = GROUND;
				      	map[xPlayer][yPlayer+1] = PLAYER;
		  			break;
		}
	      break;
	    }
	        SDL_UpdateWindowSurface(window);
	}
    }

  	//clean
//	SDL_FreeSurface(goal);
  	SDL_FreeSurface(screen);
	SDL_DestroyWindow(window);
	SDL_Quit();
  	return EXIT_SUCCESS;
}
