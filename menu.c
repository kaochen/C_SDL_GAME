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

#ifndef MENU_C
#define MENU_C

#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "const.h"

int levelSelector(SDL_Window *window, SDL_Surface *screen)
{
	//create a menu background surface
  	SDL_Surface *menuBackground = NULL;
  	SDL_Rect menuPos;
	menuBackground = SDL_CreateRGBSurface(0, MENU_WIDTH, MENU_HEIGHT, 32, 0,255,0,0);
	menuPos.x = ((W_HEIGHT - MENU_HEIGHT)/2);
  	menuPos.y = ((W_WIDTH - MENU_WIDTH)/2);
	SDL_FillRect(menuBackground, NULL, SDL_MapRGB(screen->format,255,0,0));
	SDL_BlitSurface(menuBackground, NULL, screen, &menuPos);
	fprintf(stderr, "Blit menu");


  	//create button1:
    	SDL_Surface *level1 = NULL;
  	SDL_Rect level1Pos;
  	level1 = SDL_CreateRGBSurface(0, BUTTON_WIDTH, BUTTON_HEIGHT, 32, 0,255,0,0);
    	level1Pos.x = (menuPos.x + (MENU_WIDTH - BUTTON_WIDTH)/2);
  	level1Pos.y = (menuPos.y + (MENU_HEIGHT - 2*BUTTON_HEIGHT)/2);
  	SDL_FillRect(level1, NULL, SDL_MapRGB(screen->format,0,255,0));
  	SDL_BlitSurface(level1, NULL, screen, &level1Pos);

    	//create button2:
    	SDL_Surface *level2 = NULL;
  	SDL_Rect level2Pos;
  	level2 = SDL_CreateRGBSurface(0, BUTTON_WIDTH, BUTTON_HEIGHT, 32, 0,255,0,0);
    	level2Pos.x = (menuPos.x + (MENU_WIDTH - BUTTON_WIDTH)/2);
  	level2Pos.y = (menuPos.y + (MENU_HEIGHT+10)/2);
  	SDL_FillRect(level2, NULL, SDL_MapRGB(screen->format,0,255,0));
  	SDL_BlitSurface(level2, NULL, screen, &level2Pos);

  	//refresh window
  	SDL_UpdateWindowSurface(window);

  int levelChoice = 0;
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
	  SDL_DestroyWindow(window);
	  SDL_Quit();
	  break;
	case SDL_KEYDOWN:
	//listen keyboard:
	switch(event.key.keysym.sym)
	    {
	    case SDLK_UP:
	  	SDL_FillRect(level1, NULL, SDL_MapRGB(screen->format,0,200,0));
	  	SDL_BlitSurface(level1, NULL, screen, &level1Pos);
	      	//reverse
	      	SDL_FillRect(level2, NULL, SDL_MapRGB(screen->format,0,255,0));
	  	SDL_BlitSurface(level2, NULL, screen, &level2Pos);
	      	levelChoice = 0;
	      	break;
	    case SDLK_DOWN:
	      	SDL_FillRect(level1, NULL, SDL_MapRGB(screen->format,0,255,0));
	  	SDL_BlitSurface(level1, NULL, screen, &level1Pos);
	      	//reverse
	      	SDL_FillRect(level2, NULL, SDL_MapRGB(screen->format,0,200,0));
	  	SDL_BlitSurface(level2, NULL, screen, &level2Pos);
	      	levelChoice = 1;
	      break;
	    case SDLK_RETURN:
	      	carryOn = 0;
	      break;
	    }
	SDL_UpdateWindowSurface(window);
	  break;

	}

  }
	SDL_FreeSurface(menuBackground);
  	SDL_FreeSurface(level1);
  	SDL_FreeSurface(level2);
    	return levelChoice;
}
#endif

