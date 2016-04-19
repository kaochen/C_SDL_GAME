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

#ifndef SPRITES_C
#define SPRITES_C
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../inc/const.h"
#include "../inc/sprites.h"

/* load a sprite as a surface */
void loadSprite (Sprites * sprites, const char *imagePath)
{
   sprites->image = IMG_Load (imagePath);
   if (sprites->image == NULL)
   {
      fprintf (stderr, "IMG_Load %s failed: %s\n", imagePath,
               SDL_GetError ());
      exit (EXIT_FAILURE);
   }
   sprites->height = 40;
   sprites->width = 40;
}

/* free all surfaces */
void freeSprites (Sprites tableSurface[NBR_OF_IMAGES])
{
   int i = 0;
   for (i = 0; i < NBR_OF_IMAGES; i++)
   {
      SDL_FreeSurface (tableSurface[i].image);
   }
}

#endif
