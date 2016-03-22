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

#define GAME_NAME "My Game"
int w_width = 640;
int w_height = 480;

void pause();


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
                          SDL_WINDOW_OPENGL);
    	//set an window icon
	SDL_SetWindowIcon(window, SDL_LoadBMP("img/icon.bmp"));

  	//Create a surface form the window
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
	SDL_BlitSurface(background, NULL, screen, &backgroundPos);



	//initiate surface one
	SDL_Surface *s = NULL;
  	SDL_Rect position;

	/* Creating the surface. */
	s = SDL_CreateRGBSurface(0, 220, 180, 32, 0, 255, 0, 0);
	/* Filling the surface with red color. */
	SDL_FillRect(s, NULL, SDL_MapRGB(s->format,127,36,4));
  	position.x = 10;
  	position.y = 10;
	SDL_BlitSurface(s, NULL, screen, &position);

	//initiate surface two
  	SDL_Surface *s2 = NULL;
  	SDL_Rect position2;

	/* Creating the surface two. */
	s2 = SDL_CreateRGBSurface(0, 200, 160, 32, 0, 255, 0, 0);
	/* Filling the surface with red color. */
	SDL_FillRect(s2, NULL, SDL_MapRGB(s2->format,255, 67, 0 ));
  	position2.x = 20;
  	position2.y = 20;
	SDL_BlitSurface(s2, NULL, screen, &position2);


  	//a gradient
	SDL_Surface *gradient = NULL;
  	SDL_Rect gradientPos;
  	gradientPos.x = 200;
  	gradientPos.y = 220;
  	//generate a simple ligne
	int i = 1;
  	int red = 255;
	int green = 0;
	int blue = 0;
  	for(i = 0; i <= 62 ; i++)
	  {
  		gradient = SDL_CreateRGBSurface(0, 1, 125, 32, 0, 255, 0, 0);
	    	SDL_FillRect(gradient, NULL, SDL_MapRGB(gradient->format,red, (green + i*2), (blue + i*2) ));
	    	gradientPos.x++;
	    	SDL_BlitSurface(gradient, NULL, screen, &gradientPos);
	 }

    	for(i = 63; i <= 125 ; i++)
	  {
  		gradient = SDL_CreateRGBSurface(0, 1, 125, 32, 0, 255, 0, 0);
	    	SDL_FillRect(gradient, NULL, SDL_MapRGB(gradient->format,red, (green - i*2), (blue - i*2) ));
	    	gradientPos.x++;
	    	SDL_BlitSurface(gradient, NULL, screen, &gradientPos);
	 }


	//refresh the window
  	SDL_UpdateWindowSurface(window);

	//wait for quit event
  	pause();
  	//clean
	SDL_DestroyWindow(window);
	SDL_Quit();
  	return EXIT_SUCCESS;
}


void pause()
{
  int carryOn = 1;
  SDL_Event event;
  while(carryOn)
    {
      SDL_WaitEvent(&event);
      switch(event.type)
	{
	case SDL_QUIT:
	  carryOn = 0;
	}
    }
}

