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
  	int map[NBR_OF_BLOCKS][NBR_OF_BLOCKS] = {0};
  	map[1][12] = WOODEN_CASE;
	map[9][9] = WOODEN_CASE;
  	map[5][7] = WOODEN_CASE;
    	map[5][8] = WALL;
      	map[4][9] = WALL;

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
	//create a woode case surface
  	SDL_Surface *woodenCase = NULL;
  	SDL_Rect woodenCasePos;
	//load image
	woodenCase = SDL_LoadBMP("img/box.bmp");

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
			}
		  }
	  }


	//refresh the window
  	SDL_UpdateWindowSurface(window);

	//wait for quit event
  int carryOn = 1;
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
	  switch(event.key.keysym.sym)
	    {
	    case SDLK_RIGHT:
          	//wall_bmpPos.x++;
	      break;
	    case SDLK_LEFT:
          	//wall_bmpPos.x--;
	      break;
	    case SDLK_UP:
          	//wall_bmpPos.y--;
	      break;
	    case SDLK_DOWN:
          	//wall_bmpPos.y++;
	      break;
	    case SDL_MOUSEBUTTONUP:
	      	//wall_bmpPos.x = event.button.x;
	      	//wall_bmpPos.y = event.button.y;
	      break;
	    }
	        //SDL_BlitSurface(wall_bmp, NULL, screen, &wall_bmpPos);
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
