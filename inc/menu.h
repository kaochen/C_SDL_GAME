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

#ifndef MENU_H
#define MENU_H

#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_rotozoom.h>

#include "const.h"
#include "level_decor.h"
#include "menu_text.h"
#include "settings.h"
#include "slc.h"

/*define menu object*/
enum
{M_EMPTY, M_PREVIOUS, M_NEXT, M_RESET, M_BACKWARDS, M_INFO, M_SHORTCUTS,
	 M_ABOUT, M_FILE, M_SETTINGS};



enum {MAIN_WINDOW, TOPBAR, TABS}; //Main elements of the interface
enum {TAB_EMPTY, INFO, SHORTCUTS, ABOUT, FILES, SETTINGS}; //Different tabs


/* Init menu grid */
void
gridMenu_init(S_Menu gridMenu[pref.menu_X_Blocks][pref.menu_Y_Blocks]);


/* display Top Bar*/
int
displayTopBar (SDL_Surface * screen,
	       Sprites tableSurface[NBR_OF_IMAGES],
	       S_LevelList * levelList,
	       Square grid[pref.max_X_Blocks][pref.max_Y_Blocks],
               S_Menu gridMenu[pref.menu_X_Blocks][pref.menu_Y_Blocks]);

/* display background of the top bar menu */
int
backgroundTopBar (SDL_Surface * screen,
                  Sprites tableSurface[NBR_OF_IMAGES]);


/* count how many goals are need to complete the level */
int
goalLeft (Square grid[pref.max_X_Blocks][pref.max_Y_Blocks]);

/* count goals all ready achieve */
int
nbr_of_goals (Square grid[pref.max_X_Blocks][pref.max_Y_Blocks]);

/* Display Progress in the level */
int
displayProgress (Square grid[pref.max_X_Blocks][pref.max_Y_Blocks],
       		SDL_Surface * screen,
		Sprites tableSurface[NBR_OF_IMAGES]);

/* Victory or not ? */
int
levelFinished (Square grid[pref.max_X_Blocks][pref.max_Y_Blocks]);

/* display a congrats message to the winner */
int
displayCongrats (SDL_Surface * screen,
                 Sprites tableSurface[NBR_OF_IMAGES]);


/* Open And close the menu */
int
openCloseTheMenu (void);

/* Show menu List */
void
openMenu (SDL_Surface * screen,
          Sprites tableSurface[NBR_OF_IMAGES],
	  S_Text tableTextSurface[NBR_OF_TAB][NBR_OF_TAB_LINE],
          S_LevelList * levelList,
          S_Menu gridMenu[pref.menu_X_Blocks][pref.menu_Y_Blocks]);

/* display Sub menu */
int
displaySubMenu (SDL_Surface * screen,
                S_Text tableTextSurface[NBR_OF_TAB][NBR_OF_TAB_LINE],
                S_LevelList * levelList);


/* Display the background menu */
void
displayOpenMenuBackground (SDL_Surface * screen,
			   Sprites tableSurface[NBR_OF_IMAGES],
                           S_Menu gridMenu[pref.menu_X_Blocks][pref.menu_Y_Blocks]);

/* Display pattern over the text menu */
void
displayOverTextImage (SDL_Surface * screen,
		     Sprites tableSurface[NBR_OF_IMAGES]);

/* Display buttons on the top bar*/
int
displayTopBarButtons (SDL_Surface * screen,
 	              Sprites tableSurface[NBR_OF_IMAGES],
                      S_Menu gridMenu[pref.menu_X_Blocks][pref.menu_Y_Blocks]);
#endif
