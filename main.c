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
                          w_width,
			  w_height,
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




  	//add a background image
	SDL_Surface *background = NULL;
  	SDL_Rect backgroundPos;
	//load image
	background = SDL_LoadBMP("img/background.bmp");
  	backgroundPos.x = 0;
  	backgroundPos.y = 0;
	//fill the background with the minimun number of background image needed
	int h = (w_height/BACKGROUND_SIZE+1);
  	int w = (w_width/BACKGROUND_SIZE);
  	fprintf(stderr, "h %d, w %d\n", h,w);
	//first do a line and multiply it
		int i = 0;
  	for(i = 0; i <= h ; i++)
	  {
		int x = 0;
	  	for(x = 0; x <= w ; x++)
		  {
		    	background = SDL_LoadBMP("img/background.bmp");
			SDL_BlitSurface(background, NULL, screen, &backgroundPos);
				backgroundPos.x += BACKGROUND_SIZE;
		  }
		    	backgroundPos.x = 0; //weird
		  	backgroundPos.y += 240;
	}




	//load wall image
	SDL_Surface *wall_bmp = NULL;
  	SDL_Rect wall_bmpPos;
	wall_bmp =  IMG_Load("img/box.bmp");
  	//Check if image is loaded*/
  	if ( wall_bmp == NULL )
   	{
      	    fprintf(stderr, "Loading wall_bmp image failed: %s\n", SDL_GetError());
	    exit(EXIT_FAILURE);
    	}
	    fprintf(stderr, "Loading wall_bmp succeed\n");
    	wall_bmpPos.x = 0;
  	wall_bmpPos.y = 0;
  	SDL_BlitSurface(wall_bmp, NULL, screen, &wall_bmpPos);


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
          	wall_bmpPos.x++;
	      break;
	    case SDLK_LEFT:
          	wall_bmpPos.x--;
	      break;
	    case SDLK_UP:
          	wall_bmpPos.y--;
	      break;
	    case SDLK_DOWN:
          	wall_bmpPos.y++;
	      break;
	    case SDL_MOUSEBUTTONUP:
	      	wall_bmpPos.x = event.button.x;
	      	wall_bmpPos.y = event.button.y;
	      break;
	    }
	        SDL_BlitSurface(wall_bmp, NULL, screen, &wall_bmpPos);
	        SDL_UpdateWindowSurface(window);
	}
    }
  	//pause();
  	//clean
	SDL_FreeSurface(wall_bmp);
	SDL_FreeSurface(background);
	SDL_DestroyWindow(window);
	SDL_Quit();
  	return EXIT_SUCCESS;
}
