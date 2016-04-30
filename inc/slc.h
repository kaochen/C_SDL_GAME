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

//Read using libxml
#include <libxml2/libxml/tree.h>
#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/xpath.h>
#include <libxml2/libxml/xpathInternals.h>

/* level definion struct */
typedef struct S_Level S_Level;
struct S_Level
{
   char name[MAX_CARACT];
   char file[MAX_CARACT];
   int height;
   int width;
   int number;
   S_Level *next;
};

/* control level list */
typedef struct S_LevelList S_LevelList;
struct S_LevelList
{
   S_Level *first;
};

/* Initiatlize the list of levels */
S_LevelList *initLevelList();

/*Add a level in the list*/
void addNewLevel(S_LevelList *levelList, char *name, int height, int width);

/* read level list one by one */
void readLevelList(S_LevelList *levelList);

int readXML(char *nameLevel);

#endif
