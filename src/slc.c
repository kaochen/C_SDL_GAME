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

#ifndef SLC_C
#define SLC_C

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>

#include "../inc/slc.h"
#include "../inc/const.h"
#include "../inc/game.h"

//Read using libxml
#include <libxml2/libxml/tree.h>
#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/xpath.h>
#include <libxml2/libxml/xpathInternals.h>



/*Manage slc files ==================================================*/
/* Initiatlize the list of files */
S_FilesList *initFilesList()
{
    S_FilesList *filesList = malloc(sizeof(*filesList));

    if (filesList == NULL) {
	fprintf(stderr, "Init files list failed\n");
	exit(EXIT_FAILURE);
    }
    filesList->first = NULL;
    filesList->last = NULL;
    filesList->nbr_of_files = 0;
    return filesList;
}

/*Add the first file in a empty list*/
int addFirstFile(S_FilesList * filesList, char *name)
{
    S_Files *firstFiles = malloc(sizeof(*firstFiles));
    if (filesList == NULL || firstFiles == NULL) {
	fprintf(stderr, "Init first file failed\n");
	return EXIT_FAILURE;
    }
    /*update first file data */
    strcpy(firstFiles->name, name);
    firstFiles->previous = NULL;
    firstFiles->next = NULL;

    /*update list files */
    filesList->first = firstFiles;
    filesList->last = firstFiles;
    filesList->nbr_of_files++;

    return EXIT_SUCCESS;
}


/*Add a new file in the list*/
void addNewFile(S_FilesList * filesList, char *name)
{
    S_Files *new = malloc(sizeof(*new));
    if (filesList == NULL || new == NULL) {
	exit(EXIT_FAILURE);
    }
    strcpy(new->name, name);

    /* insert at the end of the chain */
    new->next = NULL;
    new->previous = filesList->last;
    filesList->last->next = new;
    filesList->last = new;
    filesList->nbr_of_files++;
}

/*Erase a file in the list from start*/
int delFile(S_FilesList * filesList)
{
    if (filesList->nbr_of_files == 0)
	return EXIT_FAILURE;
    S_Files *del_file = malloc(sizeof(*del_file));
    del_file = filesList->first;
    filesList->first = filesList->first->next;
    if (filesList->nbr_of_files == 1)
	filesList->last = NULL;

    free(del_file->name);
    free(del_file);
    filesList->nbr_of_files--;
    return EXIT_SUCCESS;
}

/* destroy file list */
void destroyFileList(S_FilesList * filesList)
{
    while (filesList->nbr_of_files > 0)
	delFile(filesList);
}

/*Load slc level into the grid */
int listSlcLevelFiles(S_FilesList * filesList)
{
    DIR *rep;
    rep = opendir("levels/");
    struct dirent *file = NULL;
    if (rep == NULL) {
	return (EXIT_FAILURE);
    }

    /*search for slc files */
    char path[MAX_CARACT] = "";
    while ((file = readdir(rep)) != NULL) {
	if (strstr(file->d_name, ".slc") != NULL
	    || strstr(file->d_name, ".SLC") != NULL) {
	    sprintf(path, "levels/%s", file->d_name);
	    if (filesList->nbr_of_files == 0) {
		addFirstFile(filesList, path);
	    } else {
		addNewFile(filesList, path);
	    }
	    fprintf(stderr, "Found file %d : %s\n",
		    filesList->nbr_of_files, path);
	}
    }

    if (closedir(rep) == EXIT_FAILURE) {
	return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

/* Read the files list one element by one */
int readFilesList(S_FilesList * filesList)
{
    if (filesList == NULL) {
	return EXIT_FAILURE;
    }
    fprintf(stderr, "Read Files List: \n");
    S_Files *actual = filesList->first;
    fprintf(stderr, "The file list contain %d files\n",
	    filesList->nbr_of_files);

    /*Read file one by one until you'll get a NULL */
    int i = 1;
    while (actual != NULL) {
	fprintf(stderr, "File %d : %s\n", i, actual->name);
	actual = actual->next;
	i++;
    }
    return EXIT_SUCCESS;
}

/*Manage levels ==================================================*/
/* Initialize the list of levels */
S_LevelList *initLevelList()
{
    S_LevelList *levelList = malloc(sizeof(*levelList));
    if (levelList == NULL) {
	exit(EXIT_FAILURE);
    }

    levelList->first = NULL;
    levelList->last = NULL;
    levelList->nbr_of_levels = 0;

    return levelList;
}

/*Add first level into an empty list */
int addFirstLevel(S_LevelList * levelList, char *fileName, char *name,
		  int height, int width)
{
    S_Level *firstLevel = malloc(sizeof(*firstLevel));
    if (firstLevel == NULL) {
	fprintf(stderr, "Add first level failed\n");
	return EXIT_FAILURE;
    }

    /*update first file data */
    strcpy(firstLevel->name, name);
    strcpy(firstLevel->fileName, fileName);
    firstLevel->height = height;
    firstLevel->width = width;

    firstLevel->previous = levelList->first;
    firstLevel->next = levelList->last;

    /*update  levelList */
    levelList->first = firstLevel;
    levelList->last = firstLevel;
    levelList->nbr_of_levels++;
    return EXIT_SUCCESS;
}

/*Add a level in the list*/
void addNewLevel(S_LevelList * levelList, char *fileName, char *name,
		 int height, int width)
{
    S_Level *new = malloc(sizeof(*new));
    if (levelList == NULL || new == NULL) {
	fprintf(stderr, "Add new level failed\n");
	exit(EXIT_FAILURE);
    }
    /* check size and */
    if (height <= X_BLOCKS && width <= Y_BLOCKS) {
	strcpy(new->name, name);
	strcpy(new->fileName, fileName);
	new->height = height;
	new->width = width;

	/* insert at the end of the chain */
	new->next = NULL;
	new->previous = levelList->last;

	levelList->last->next = new;
	levelList->last = new;
	levelList->nbr_of_levels++;
    }
}

/*Erase a level in the list from start*/
int delLevel(S_LevelList * levelList)
{
    if (levelList->nbr_of_levels == 0)
	return EXIT_FAILURE;
    S_Level *del_level = malloc(sizeof(*del_level));
    del_level = levelList->first;
    levelList->first = levelList->first->next;
    if (levelList->nbr_of_levels == 1)
	levelList->last = NULL;

    free(del_level->name);
    levelList->nbr_of_levels--;
    return EXIT_SUCCESS;
}

/* destroy level list */
void destroy(S_LevelList * levelList)
{
    while (levelList->nbr_of_levels > 0)
	delLevel(levelList);
}

/*get levels infos from files */
int getNbrOfLevels(S_LevelList * levelList)
{
    int i = 0;
    S_Level *actual = malloc(sizeof(*actual));
    if (levelList == NULL || actual == NULL) {
	perror("getNbrOfLevels");
	exit(EXIT_FAILURE);
    }
    actual = levelList->first;
    while (actual->name != NULL) {
	actual = actual->next;
	i++;
    }
    fprintf(stderr, "Found %d levels\n", i);
    return i;
}

/*get levels infos from files */
int readLevelsAttributs(S_FilesList * filesList, S_LevelList * levelList)
{
    S_Level *new = malloc(sizeof(*new));
    if (filesList == NULL || levelList == NULL || new == NULL) {
	fprintf(stderr, "readLevelsAttributs failed\n");
	return EXIT_FAILURE;
    }

    /*Get files names */
    xmlDocPtr doc;
    S_Files *actualFile = filesList->first;
    while (actualFile != NULL) {

	/* Read each level from each files */

	fprintf(stderr, "Read levels from : %s\n", actualFile->name);

	/* Open SLC/XML file */
	doc = xmlParseFile(actualFile->name);
	if (doc == NULL) {
	    fprintf(stderr, "%s not valid\n", actualFile->name);
	    return EXIT_FAILURE;
	}
	// Start XPath
	xmlXPathInit();
	// Create a context
	xmlXPathContextPtr ctxt = xmlXPathNewContext(doc);
	if (ctxt == NULL) {
	    fprintf(stderr, "Error creating the context XPath\n");
	    return EXIT_FAILURE;
	}

	/* Read Level */
	xmlXPathObjectPtr xpathLevel =
	    xmlXPathEvalExpression(BAD_CAST
				   "/SokobanLevels/LevelCollection/Level",
				   ctxt);
	if (xpathLevel == NULL) {
	    fprintf(stderr, "Error on the xPathLevel expression\n");
	    return EXIT_FAILURE;
	}

	/*get attributs */
	xmlChar *name;
	xmlChar *width;
	xmlChar *height;
	/*Get the number of levels in a file */
	int levelCount = 0, i = 0;
	levelCount = xpathLevel->nodesetval->nodeNr;
	fprintf(stderr, "The files %s contain %d levels\n\n",
		actualFile->name, levelCount);
	/*Add S_Level for each levels found */
	while (i < levelCount) {
	    xmlNodePtr Node = xpathLevel->nodesetval->nodeTab[i];
	    for (xmlAttrPtr attr = Node->properties; NULL != attr;
		 attr = attr->next) {
		name = xmlGetProp(Node, (xmlChar *) "Id");
		width = xmlGetProp(Node, (xmlChar *) "Width");
		height = xmlGetProp(Node, (xmlChar *) "Height");
	    }
	    /*Load infos into the levelList */
	    if (levelList->nbr_of_levels == 0) {
		addFirstLevel(levelList, actualFile->name, (char*)name,
			      atoi((char*)height),atoi((char*)width));
	    } else {
		addNewLevel(levelList, actualFile->name, (char*)name,
			      atoi((char*)height),atoi((char*)width));
	    }
	    //printf ("File: %s, name: %s, width: %s, height: %s\n",actualFile->name, name, width, height);
	    i++;
	}
	/* free memory */
	xmlXPathFreeContext(ctxt);
	xmlFreeDoc(doc);

	actualFile = actualFile->next;
    }
    return EXIT_SUCCESS;
}

/* read level list one by one */
void readLevelList(S_LevelList * levelList)
{
    if (levelList == NULL) {
	exit(EXIT_FAILURE);
    }
    S_Level *actual = levelList->first;
    while (actual != NULL) {
	fprintf(stderr, "%s, %d:%d\n", actual->name, actual->width,
		actual->height);
	actual = actual->next;
    }
}

/*Load slc level into the grid */
int loadSlcLevel(int levelChoice, S_LevelList * levelList,
		 Square grid[][MAX_BLOCKS])
{
    if (levelList == NULL) {
	return EXIT_FAILURE;
    }
    int nbr_of_lines = 0, firstLines = 0, nbr_of_columns = 0, firstColumn =
	0, i = 0;
    S_Level *actual = levelList->first;
    /* read the all chain list */
    while (actual != NULL) {
	/* try to find the nameLevel into the list */
	if (i == levelChoice) {
	    fprintf(stderr, "Found %s, %d:%d\n", actual->name,
		    actual->width, actual->height);
	    nbr_of_lines = actual->height;
	    nbr_of_columns = actual->width;
	    /*Place the drawing into the center of the grid */
	    firstLines = ((Y_BLOCKS - nbr_of_lines) / 2 + 1);	//+1 for the menu
	    firstColumn = (X_BLOCKS - nbr_of_columns) / 2;
	    break;
	}
	i++;
	actual = actual->next;
    }

    xmlDocPtr doc;

    /* Open SLC/XML file */
    doc = xmlParseFile(actual->fileName);
    if (doc == NULL) {
	fprintf(stderr, "XML Document not valid\n");
	return EXIT_FAILURE;
    }
    // Start XPath
    xmlXPathInit();
    // Create a context
    xmlXPathContextPtr ctxt = xmlXPathNewContext(doc);
    if (ctxt == NULL) {
	fprintf(stderr, "Error creating the context XPath\n");
	return EXIT_FAILURE;
    }

    /* Read Level */
    char path[MAX_CARACT] = "";
    sprintf(path,
	    "/SokobanLevels/LevelCollection/Level[@Id=\"%s\"]/L/text()",
	    actual->name);
    xmlXPathObjectPtr xpathLevel =
	xmlXPathEvalExpression(BAD_CAST path, ctxt);
    if (xpathLevel == NULL) {
	fprintf(stderr, "Error on the xPathLevel expression\n");
	return EXIT_FAILURE;
    }
    /*Clean grid before */
    int y = 0, x = 0;
    for (y = 0; y < Y_BLOCKS; y++) {
	for (x = 0; x < X_BLOCKS; x++) {
	    grid[x][y].mainRole = GROUND;
	    grid[x][y].objectType = EMPTY;
	}
    }
    /* load level into the grid */
    if (xpathLevel->type == XPATH_NODESET) {
	int c = 0;
	y = 0, x = 0;
	char line[MAX_CARACT] = "";
	xmlNodePtr n;
	for (y = 0; y < nbr_of_lines; y++) {
	    n = xpathLevel->nodesetval->nodeTab[y];
	    if (n->type == XML_TEXT_NODE
		|| n->type == XML_CDATA_SECTION_NODE) {
		strcpy(line, (char*)n->content);
		fprintf(stderr, "%s\n", n->content);
		/* load level into the grid */
		c = 0;
		for (x = firstColumn; x < (firstColumn + nbr_of_columns);
		     x++) {
		    int y2 = y + firstLines;
		    switch (line[c]) {
		    case '#':
			grid[x][y2].mainRole = WALL;
			break;
		    case ' ':
			grid[x][y2].mainRole = GROUND;
			break;
		    case '$':
			grid[x][y2].mainRole = BOX;
			break;
		    case '*':
			grid[x][y2].mainRole = BOX;
			grid[x][y2].objectType = GOAL;
			break;
		    case '.':
			grid[x][y2].mainRole = GOAL;
			grid[x][y2].objectType = GOAL;
			break;
		    case '@':
			grid[x][y2].mainRole = PLAYER;
			break;
		    case '+':
			grid[x][y2].mainRole = PLAYER;
			break;
		    }
		    c++;
		}
	    }
	}
    }

    /* Change grounds that are outside the walls to the OUTSIDE flag */
    blitOutside(grid);

    detectCorner(grid);
    randomOutside(grid);


    /* free memory */
    xmlFreeDoc(doc);
    xmlXPathFreeContext(ctxt);
    // xmlXPathFreeObject(xpathLevel); //Doesn't work ???

    return EXIT_SUCCESS;
}

/* Change grounds that are outiside the walls to outsides */
void blitOutside(Square grid[][MAX_BLOCKS])
{
    /*Read line by line and Left to Right */
    int x = 0, y = 0;
    for (y = 0; y < Y_BLOCKS; y++) {
	for (x = 0; x < X_BLOCKS; x++) {
	    /*break if wall */
	    if (grid[x][y].mainRole == WALL)
		x = X_BLOCKS;

	    /*Change GROUND to OUTSIDE */
	    if (grid[x][y].mainRole == GROUND) {
		grid[x][y].mainRole = OUTSIDE;
	    }
	}
    }

    /*Read line by line Right to Left */
    x = 0, y = 0;
    for (y = 0; y < Y_BLOCKS; y++) {
	for (x = X_BLOCKS; x > -1; x--) {
	    /*break if wall */
	    if (grid[x][y].mainRole == WALL)
		x = -1;

	    /*Change GROUND to OUTSIDE */
	    if (grid[x][y].mainRole == GROUND) {
		grid[x][y].mainRole = OUTSIDE;
	    }
	}
    }

    /*Read row by row and Top to Bottom */
    x = 0;
    y = 0;
    for (x = 0; x < X_BLOCKS; x++) {
	for (y = 0; y < Y_BLOCKS; y++) {
	    /*break if wall */
	    if (grid[x][y].mainRole == WALL)
		y = Y_BLOCKS;

	    /*If a ground is outside the wall use OUTSIDE */
	    if (grid[x][y].mainRole == GROUND) {
		grid[x][y].mainRole = OUTSIDE;
	    }
	}
    }

    /*Read row by row and Bottom to Top */
    x = 0;
    y = 0;
    for (x = 0; x < X_BLOCKS; x++) {
	for (y = Y_BLOCKS; y > 0; y--) {
	    /*break if wall */
	    if (grid[x][y].mainRole == WALL)
		y = 0;

	    /*If a ground is outside the wall use OUTSIDE */
	    if (grid[x][y].mainRole == GROUND) {
		grid[x][y].mainRole = OUTSIDE;
	    }

	}
    }

}

#endif
