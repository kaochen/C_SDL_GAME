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
{M_EMPTY, M_PREVIOUS, M_NEXT, M_RESET, M_BACKWARDS, M_INFO, M_SHORTCUTS, M_ABOUT, M_FILE
};

/*menuChoice*/
enum{
MC_TOPBAR, MC_INFO, MC_SHORTCUTS, MC_ABOUT, MC_FILE
};

enum {
SUB_EMPTY, TOPBAR, OPENMENU,
INFO,SHORTCUTS,ABOUT,FILES, //main menu
INFO1,INFO2,INFO3,
SHORTCUTS1,SHORTCUTS2,SHORTCUTS3,SHORTCUTS4,SHORTCUTS5,
ABOUT1,ABOUT2,
FILES1,FILES2,FILES3,FILES4
};

typedef struct
{
 int role;
 int type;
 int text;
 int menu;
 int x;
 int y;
}S_Menu;

typedef struct
{
 int tab;
 int sub; //under tabs
 int max; //max lines in sub part
 int open; //the menu panel is open or not
 int freeze; //Freeze the gameplay when menu or congrats are open
}S_menuchoice;

/* Init menu grid */
S_Menu *
initGridMenu(void);

/* display Top Bar*/
int
displayTopBar (int levelNumber, SDL_Surface * screen,
	       Sprites tableSurface[NBR_OF_IMAGES],
	       S_LevelList * levelList,
	       Square grid[getMax_X_Blocks ()][getMax_Y_Blocks ()],
               S_Menu gridMenu[MENU_LINE]);

/* display background top bar */
int backgroundTopBar (SDL_Surface * screen,
		     Sprites tableSurface[NBR_OF_IMAGES]);


/* count how many goals are need to complete the level */
int goalLeft (Square grid[getMax_X_Blocks ()][getMax_Y_Blocks ()]);

/* count goals all ready achieve */
int nbr_of_goals (Square grid[getMax_X_Blocks ()][getMax_Y_Blocks ()]);

/* Display Progress in the level */
int displayProgress (Square grid[getMax_X_Blocks ()][getMax_Y_Blocks ()], SDL_Surface * menu,
		      Sprites tableSurface[NBR_OF_IMAGES]);

/* Victory or not ? */
int levelFinished (Square grid[getMax_X_Blocks ()][getMax_Y_Blocks ()]);

/* display a congrats message to the winner */
int displayCongrats (SDL_Surface * screen,
		     Sprites tableSurface[NBR_OF_IMAGES]);

/* return the height of the menu in blocks*/
int menuHeight (void);


/* menu position*/
int menuPosX (void);

/* Open And close the menu */
int
openCloseTheMenu (S_menuchoice *menuChoice);

/* Show menu List */
void
openMenu (SDL_Surface * screen, Sprites tableSurface[NBR_OF_IMAGES],
	  S_Text tableTextSurface[NBR_OF_TEXT], S_LevelList * levelList,
	  S_menuchoice menuChoice, int levelChoice, S_Menu gridMenu[MENU_LINE]);

/* display Sub menu */
int displaySubMenu (SDL_Surface * screen, S_Text tableTextSurface[NBR_OF_TEXT], S_LevelList * levelList, S_menuchoice menuChoice, int levelChoice);

/* Display the background menu */
void
displayOpenMenuBackground(SDL_Surface * screen, Sprites tableSurface[NBR_OF_IMAGES], S_menuchoice menuChoice);

/* Display pattern over the text menu */
void
displayOverTextImage(SDL_Surface * screen, Sprites tableSurface[NBR_OF_IMAGES], S_menuchoice menuChoice);

/* Display buttons on the top bar*/
int
displayTopBarButtons (SDL_Surface * screen,
		      Sprites tableSurface[NBR_OF_IMAGES], S_Menu gridMenu[MENU_LINE]);
#endif
