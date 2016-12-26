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

#ifndef MENU_TEXT_H
#define MENU_TEXT_H

#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_rotozoom.h>

#include "const.h"
#include "level_decor.h"
#include "menu.h"
#include "settings.h"
#include "slc.h"



/* define images/sprites struct */
typedef struct
{
  SDL_Surface *image;
  size_t tabName;
  size_t lineNbr;
} S_Text;

/*init the textSurface table */
void
tableTextSurface_init (S_Text tableTextSurface[NBR_OF_TAB][NBR_OF_TAB_LINE]);

/* free all text surfaces */
void
freeS_Text (S_Text tableTextSurface[NBR_OF_TAB][NBR_OF_TAB_LINE]);

/*make from a text a table and load it into the tableTextSurface*/
void
loadTextAsSurface (size_t tab, size_t lineNbr, S_Text tableTextSurface[NBR_OF_TAB][NBR_OF_TAB_LINE], char *text,
		   size_t fontSize, size_t R, size_t G, size_t B, size_t A);


/* display the level number */
int
levelMenu (SDL_Surface * screen,
           S_LevelList * levelList);

#endif
