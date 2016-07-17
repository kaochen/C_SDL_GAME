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
#include "game.h"
#include "settings.h"

//Read using libxml
#include <libxml2/libxml/tree.h>
#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/xpath.h>
#include <libxml2/libxml/xpathInternals.h>


/* Structures ============================================ */

/* Files ========================================= */
/* files definion struct */
typedef struct S_Files S_Files;
struct S_Files {
    char name[MAX_CARACT];
    S_Files *next;
    S_Files *previous;
};

/* control files list */
typedef struct S_FilesList S_FilesList;
struct S_FilesList {
    S_Files *first;
    S_Files *last;
    int nbr_of_files;
};

/* Levels ========================================= */

/* level definion struct */
typedef struct S_Level S_Level;
struct S_Level {
    char name[MAX_CARACT];
    char fileName[MAX_CARACT];
    int height;
    int width;
    S_Level *next;
    S_Level *previous;
};

/* control level list */
typedef struct S_LevelList S_LevelList;
struct S_LevelList {
    S_Level *first;
    S_Level *last;
    int nbr_of_levels;
};

/* Functions ============================================ */

/* Files ========================================= */
/* Initiatlize the list of files */
S_FilesList *initFilesList();

/*Add the first file in a empty list*/
int addFirstFile(S_FilesList * filesList, char *name);

/*Add a new file in the list*/
void addNewFile(S_FilesList * filesList, char *name);

/*Erase a file in the list from start*/
int delFile(S_FilesList * filesList);

/* destroy file list */
void destroyFileList(S_FilesList * filesList);

/* list slc files from the levels folder */
int listSlcLevelFiles(S_FilesList * filesList);

/* Read the files list one element by one */
int readFilesList(S_FilesList * filesList);

/* Levels ========================================= */
/* Initiatlize the list of levels */
S_LevelList *initLevelList();

/*get levels infos from files */
int readLevelsAttributs(S_FilesList * filesList, S_LevelList * levelList);

/*Add a level in the list*/
void addNewLevel(S_LevelList * levelList, char *fileName, char *name,
		 int height, int width);

/*Erase a level in the list from start*/
int delLevel(S_LevelList * levelList);

/* destroy level list */
void destroy(S_LevelList * levelList);

/* read level list one by one */
void readLevelList(S_LevelList * levelList);

/*Load slc level into the grid */
int loadSlcLevel(int levelChoice, S_LevelList * levelList,
		 Square grid[][getMax_Blocks()]);

/*get levels infos from files */
int getNbrOfLevels(S_LevelList * levelList);

/* Change GROUND that are outside the walls to OUTSIDE */
void detectOutside(Square grid[][getMax_Blocks()]);

#endif
