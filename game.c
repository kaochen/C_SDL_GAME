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

//create a background
void displayBackground(SDL_Surface *screen, SDL_Surface *background)
{
  	//add a background image

  	SDL_Rect backgroundPos;
	//load image
	background = SDL_LoadBMP("img/background.bmp");
  	backgroundPos.x = 0;
  	backgroundPos.y = 0;
	//fill the background with the minimun number of background image needed
	int h = (W_HEIGHT/BACKGROUND_SIZE+1);
  	int w = (W_WIDTH/BACKGROUND_SIZE);
  	fprintf(stderr, "h %d, w %d\n", h,w);
	//first do a line and multiply it
		int i = 0;
  	for(i = 0; i <= h ; i++)
	  {
		//build a line by repeating the background
	    	int x = 0;
	  	for(x = 0; x <= w ; x++)
		  {
		    	background = SDL_LoadBMP("img/background.bmp");
			SDL_BlitSurface(background, NULL, screen, &backgroundPos);
				backgroundPos.x += BACKGROUND_SIZE;
		  }
		    	backgroundPos.x = 0; //restart left
		  	backgroundPos.y += BACKGROUND_SIZE; //jump to next line position
	}

}

#endif
