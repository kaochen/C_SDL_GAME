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

#ifndef SLC_H
#define SLC_H

#include "../inc/const.h"

/* define .slc file struct */
typedef struct
{
   char filename[MAX_CARACT];
   char title[MAX_CARACT];
   char copyright[MAX_CARACT];
   char description[MAX_CARACT];

   int maxWidth;
   int maxHeight;
   int nbr_of_level;
} S_slcFile;

/* define level info extract .slc file */
typedef struct
{
   int levelNumber ;
   char levelname[MAX_CARACT];

   int width;
   int height;

} S_level;

/*open the slc file */
void openSlcFile ();

/*read one line */
void extactData(char line[MAX_CARACT]);

/*read one level */
void readLevel (int levelNum);
#endif
