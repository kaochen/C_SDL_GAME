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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>

#include "const.h"
#include "level_decor.h"
#include "settings.h"
#include "menu.h"


//Read using libxml
#include <libxml2/libxml/tree.h>
#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/xpath.h>
#include <libxml2/libxml/xpathInternals.h>



/* Functions ============================================ */

/* Files ========================================= */
/* Initiatlize the list of files */
S_FilesList *initFilesList ();

/*Add the first file in a empty list*/
int addFirstFile (S_FilesList * filesList, char *name);

/*Add a new file in the list*/
void addNewFile (S_FilesList * filesList, char *name);

/*Erase a file in the list from start*/
int delFile (S_FilesList * filesList);

/* destroy file list */
void destroyFileList (S_FilesList * filesList);

/* list slc files from the levels folder */
int listSlcLevelFiles (S_FilesList * filesList);

/* Read the files list one element by one */
int readFilesList (S_FilesList * filesList);

/* Levels ========================================= */
/* Initiatlize the list of levels */
S_LevelList *initLevelList ();

/*get levels infos from files */
int
readLevelsAttributs (S_preferences * pref,
                     S_FilesList * filesList,
                     S_LevelList * levelList);

/*Add a level in the list*/
void
addNewLevel (S_preferences *pref,
             S_LevelList * levelList,
             char *fileName,
             char *name,
             int height,
             int width);

/*Erase a level in the list from start*/
int delLevel (S_LevelList * levelList);

/* destroy level list */
void destroy (S_LevelList * levelList);

/* read level list one by one */
void readLevelList (S_LevelList * levelList);

/*Load slc level into the grid */
int
loadSlcLevel (S_preferences * pref,
              int levelChoice,
              S_LevelList * levelList,
              Square grid[pref->max_X_Blocks][pref->max_Y_Blocks],
              S_menuchoice *menuChoice);

/*get levels infos from files */
int getNbrOfLevels (S_LevelList * levelList);

/* Change GROUND that are outside the walls to OUTSIDE */
void
detectOutside (S_preferences *pref,
              Square grid[pref->max_X_Blocks][pref->max_Y_Blocks]);

#endif
