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

#ifndef LEVEL_H
#define LEVEL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "const.h"
#include "sprites.h"
#include "slc.h"

/* Init grid */
void
grid_init(Square grid[pref.max_X_Blocks][pref.max_Y_Blocks]);

/* display level on the screen */
int
displayLevel (Square grid[pref.max_X_Blocks][pref.max_Y_Blocks],
              SDL_Surface * screen,
              Sprites tableSurface[NBR_OF_IMAGES]);

/*get level name*/
int getLevelName (S_LevelList * levelList, char *levelName);

/*get level current filename*/
int
getFileName (S_LevelList * levelList, char *fileName);

/*get level current author*/
int
getAuthor (S_LevelList * levelList, char *author);

/*Trunk long char*/
int
trunkLongChar(size_t size, char * text );

/*Found the level number with just the name*/
int findLevelNumber (S_LevelList * levelList, char *levelName);

/*get a levelchoice by reading the level name from the preference file*/
int readLevelFromSetting (S_LevelList * levelList);

/*force reload of a level*/
int
changeLevel(int i);
#endif
