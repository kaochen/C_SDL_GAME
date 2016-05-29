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
    S_Files *files = malloc(sizeof(*files));
    if (filesList == NULL || files == NULL) {
	fprintf(stderr, "Init files list failed\n");
	exit(EXIT_FAILURE);
    }
    /*load first files */
    strcpy(files->name, FIRST_STRUCT);
    files->next = NULL;
    /* store adress of the first files struct */
    filesList->first = files;
    return filesList;
}

/*Add a new file in the list*/
void addNewFile(S_FilesList * filesList, char *name)
{
    S_Files *new = malloc(sizeof(*new));
    if (filesList == NULL || new == NULL) {
	exit(EXIT_FAILURE);
    }
    strcpy(new->name, name);

    /* insert new file struct into the chain */
    new->next = filesList->first;
    filesList->first = new;
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
	    addNewFile(filesList, path);
	    fprintf(stderr, "Found file :%s\n", path);
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
    int i = 1;
    /*Read file one by one until you'll find the first initiate struct (FIRST_STRUCT) or get a NULL */
    char firstCmp[] = FIRST_STRUCT, secondCmp[MAX_CARACT] = "";
    while (actual != NULL
	   && strcmp(firstCmp, strcpy(secondCmp, actual->name)) != 0) {
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
    S_Level *level = malloc(sizeof(*level));
    if (levelList == NULL || level == NULL) {
	exit(EXIT_FAILURE);
    }
    /*load first level */
    strcpy(level->name, FIRST_STRUCT);
    strcpy(level->fileName, FIRST_STRUCT);
    level->height = 0;
    level->width = 0;
    level->next = NULL;
    /* store adress of the first level */
    levelList->first = level;

    return levelList;
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
	return EXIT_FAILURE;
    }

    /*Get files names */
    xmlDocPtr doc;
    int i = 0;
    S_Files *actualFile = filesList->first;
    while (actualFile != NULL) {
	actualFile = actualFile->next;

	/* Read each level from each files */
	if (actualFile->name != NULL
	    && strcmp(actualFile->name, FIRST_STRUCT) != 0) {
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
		    name = xmlGetProp(Node, "Id");
		    width = xmlGetProp(Node, "Width");
		    height = xmlGetProp(Node, "Height");
		}
		//printf ("File: %s, name: %s, width: %s, height: %s\n",actualFile->name, name, width, height);
		/*Load infos into the levelList */

		addNewLevel(levelList, actualFile->name, name,
			    atoi(height), atoi(width));
		i++;
	    }
	    /* free memory */
	    xmlXPathFreeContext(ctxt);
	    xmlFreeDoc(doc);
	}

    }
    return EXIT_SUCCESS;
}

/*Add a level in the list*/
void addNewLevel(S_LevelList * levelList, char *fileName, char *name,
		 int height, int width)
{
    S_Level *new = malloc(sizeof(*new));
    if (levelList == NULL || new == NULL) {
	exit(EXIT_FAILURE);
    }
    /* check size and */
    if (height <= X_BLOCKS && width <= Y_BLOCKS) {
	strcpy(new->name, name);
	strcpy(new->fileName, fileName);
	new->height = height;
	new->width = width;

	/* insert new level struct into the chain */
	new->next = levelList->first;
	levelList->first = new;
    }

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
	    grid[x][y].roleType = GROUND;
	    grid[x][y].objectType = EMPTY;
	}
    }
    /* load level into the grid */
    if (xpathLevel->type == XPATH_NODESET) {
	int c = 0, i = 0;
	y = 0, x = 0;
	char line[MAX_CARACT] = "";
	xmlNodePtr n;
	for (y = 0; y < nbr_of_lines; y++) {
	    n = xpathLevel->nodesetval->nodeTab[y];
	    if (n->type == XML_TEXT_NODE
		|| n->type == XML_CDATA_SECTION_NODE) {
		strcpy(line, n->content);
		fprintf(stderr, "%s\n", n->content);
		/* load level into the grid */
		c = 0;
		for (x = firstColumn; x < (firstColumn + nbr_of_columns);
		     x++) {
		    int y2 = y + firstLines;
		    switch (line[c]) {
		    case '#':
			grid[x][y2].roleType = WALL;
			break;
		    case ' ':
			grid[x][y2].roleType = GROUND;
			break;
		    case '$':
			grid[x][y2].roleType = BOX;
			break;
		    case '*':
			grid[x][y2].roleType = BOX;
			grid[x][y2].objectType = GOAL;
			break;
		    case '.':
			grid[x][y2].roleType = GOAL;
			grid[x][y2].objectType = GOAL;
			break;
		    case '@':
			grid[x][y2].roleType = PLAYER;
			break;
		    case '+':
			grid[x][y2].roleType = PLAYER;
			break;
		    }
		    c++;
		}
	    }
	}
    }

    /* Change grounds that are outside the walls to the OUTSIDE flag */
    blitOutside(levelList, grid);

    detectCorner(grid);
    randomOutside(grid);


    /* free memory */
    xmlFreeDoc(doc);
    xmlXPathFreeContext(ctxt);
    // xmlXPathFreeObject(xpathLevel); //Doesn't work ???

    return EXIT_SUCCESS;
}

/* Change grounds that are outiside the walls to outsides */
void blitOutside(S_LevelList * levelList, Square grid[][MAX_BLOCKS])
{
    /*Read line by line and Left to Right */
    int x = 0, y = 0;
    for (y = 0; y < Y_BLOCKS; y++) {
	for (x = 0; x < X_BLOCKS; x++) {
	    /*break if wall */
	    if (grid[x][y].roleType == WALL)
		x = X_BLOCKS;

	    /*Change GROUND to OUTSIDE */
	    if (grid[x][y].roleType == GROUND) {
		grid[x][y].roleType = OUTSIDE;
	    }
	}
    }

    /*Read line by line Right to Left */
    x = 0, y = 0;
    for (y = 0; y < Y_BLOCKS; y++) {
	for (x = X_BLOCKS; x > -1; x--) {
	    /*break if wall */
	    if (grid[x][y].roleType == WALL)
		x = -1;

	    /*Change GROUND to OUTSIDE */
	    if (grid[x][y].roleType == GROUND) {
		grid[x][y].roleType = OUTSIDE;
	    }
	}
    }

    /*Read row by row and Top to Bottom */
    x = 0;
    y = 0;
    for (x = 0; x < X_BLOCKS; x++) {
	for (y = 0; y < Y_BLOCKS; y++) {
	    /*break if wall */
	    if (grid[x][y].roleType == WALL)
		y = Y_BLOCKS;

	    /*If a ground is outside the wall use OUTSIDE */
	    if (grid[x][y].roleType == GROUND) {
		grid[x][y].roleType = OUTSIDE;
	    }
	}
    }

    /*Read row by row and Bottom to Top */
    x = 0;
    y = 0;
    for (x = 0; x < X_BLOCKS; x++) {
	for (y = Y_BLOCKS; y > 0; y--) {
	    /*break if wall */
	    if (grid[x][y].roleType == WALL)
		y = 0;

	    /*If a ground is outside the wall use OUTSIDE */
	    if (grid[x][y].roleType == GROUND) {
		grid[x][y].roleType = OUTSIDE;
	    }

	}
    }

}

#endif
