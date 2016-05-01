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

//Read using libxml
#include <libxml2/libxml/tree.h>
#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/xpath.h>
#include <libxml2/libxml/xpathInternals.h>

/*Manage slc files ==================================================*/
/* Initiatlize the list of files */
S_FilesList *initFilesList ()
{
   S_FilesList *filesList = malloc (sizeof (*filesList));
   S_Files *files = malloc (sizeof (*files));
   if (filesList == NULL || files == NULL)
   {
      exit (EXIT_FAILURE);
   }
   /*load first files */
   strcpy (files->name, "");
   files->next = NULL;
   /* store adress of the first files struct */
   filesList->first = files;

   return filesList;
}

/*Add a new file in the list*/
void addNewFile (S_FilesList * filesList, char *name)
{
   S_Files *new = malloc (sizeof (*new));
   if (filesList == NULL || new == NULL)
   {
      exit (EXIT_FAILURE);
   }
   strcpy (new->name, name);

   /* insert new file struct into the chain */
   new->next = filesList->first;
   filesList->first = new;
}


void listSlcLevelFiles (S_FilesList * filesList)
{
   DIR *rep;
   rep = opendir ("levels/");
   struct dirent *file = NULL;
   if (rep == NULL)
   {
      perror ("levels ");
      exit (EXIT_FAILURE);
   }
   /*search for slc files */
   while ((file = readdir (rep)) != NULL)
   {
      if (strstr (file->d_name, ".slc") != NULL
          || strstr (file->d_name, ".SLC") != NULL)
		addNewFile(filesList, file->d_name);
         fprintf (stderr, "%s\n", file->d_name);
   }

   if (closedir (rep) == -1)
   {
      perror ("levels ");
      exit (-1);
   }
}

/* read files list one by one */
void readFilesList (S_FilesList * filesList)
{
   if (filesList == NULL)
   {
      exit (EXIT_FAILURE);
   }
   S_Files *actual = filesList->first;
   while (actual != NULL)
   {
      fprintf (stderr, "Files: %s\n", actual->name);
      actual = actual->next;
   }
}

/*Manage levels ==================================================*/
/* Initiatlize the list of levels */
S_LevelList *initLevelList ()
{
   S_LevelList *levelList = malloc (sizeof (*levelList));
   S_Level *level = malloc (sizeof (*level));
   if (levelList == NULL || level == NULL)
   {
      exit (EXIT_FAILURE);
   }
   /*load first level */
   strcpy (level->name, "");
   level->height = 0;
   level->width = 0;
   level->next = NULL;
   /* store adress of the first level */
   levelList->first = level;

   return levelList;
}

/*Add a level in the list*/
void addNewLevel (S_LevelList * levelList, char *name, int height, int width)
{
   S_Level *new = malloc (sizeof (*new));
   if (levelList == NULL || new == NULL)
   {
      exit (EXIT_FAILURE);
   }
   strcpy (new->name, name);
   new->height = height;
   new->width = width;

   /* insert new level struct into the chain */
   new->next = levelList->first;
   levelList->first = new;
}

/* read level list one by one */
void readLevelList (S_LevelList * levelList)
{
   if (levelList == NULL)
   {
      exit (EXIT_FAILURE);
   }
   S_Level *actual = levelList->first;
   while (actual != NULL)
   {
      fprintf (stderr, "%s, %d:%d\n", actual->name, actual->width,
               actual->height);
      actual = actual->next;
   }
}

/*Read a level */
int readslcLevel (char *nameLevel, S_LevelList * levelList,
                  Square grid[][MAX_BLOCKS])
{
   if (levelList == NULL)
   {
      exit (EXIT_FAILURE);
   }

   int nbr_of_lines = 0, nbr_of_columns = 0, firstColumn = 0;
   S_Level *actual = levelList->first;
   /* read the all chain list */
   while (actual != NULL)
   {
      /* try to find the nameLevel into the list */
      if (strcmp (actual->name, nameLevel) == 0)
      {
         fprintf (stderr, "Found %s, %d:%d\n", actual->name, actual->width,
                  actual->height);
         nbr_of_lines = actual->height;
         nbr_of_columns = actual->width;
         firstColumn = (X_BLOCKS - nbr_of_columns) / 2;
      }
      actual = actual->next;
   }

   xmlDocPtr doc;

   /* Open SLC/XML file */
   doc = xmlParseFile ("levels/Alberto_Garcia_Arranged.slc");
   if (doc == NULL)
   {
      fprintf (stderr, "XML Document not valid\n");
      return 0;
   }

   // Start XPath
   xmlXPathInit ();
   // Create a context
   xmlXPathContextPtr ctxt = xmlXPathNewContext (doc);
   if (ctxt == NULL)
   {
      fprintf (stderr, "Error creating the context XPath\n");
      exit (-1);
   }

   /* Read Level */
   char path[MAX_CARACT] = "";
   sprintf (path, "/SokobanLevels/LevelCollection/Level[@Id=\"%s\"]/L/text()",
            nameLevel);
   xmlXPathObjectPtr xpathLevel =
      xmlXPathEvalExpression (BAD_CAST path, ctxt);
   if (xpathLevel == NULL)
   {
      fprintf (stderr, "Error on the xPathLevel expression\n");
      exit (-1);
   }
   /*Clean grid before */
   int y = 0, x = 0;
   for (y = 0; y < Y_BLOCKS; y++)
   {
      for (x = 0; x < X_BLOCKS; x++)
      {
         grid[x][y].roleType = GROUND;
      }
   }
   /* load level into the grid */
   if (xpathLevel->type == XPATH_NODESET)
   {
      int c = 0;
      y = 0, x = 0;
      char line[MAX_CARACT] = "";
      xmlNodePtr n;
      printf ("Level: ");
      for (y = 0; y < nbr_of_lines; y++)
      {
         n = xpathLevel->nodesetval->nodeTab[y];
         if (n->type == XML_TEXT_NODE || n->type == XML_CDATA_SECTION_NODE)
         {
            strcpy (line, n->content);
            fprintf (stderr, "%s\n", n->content);
            /* load level into the grid */
            c = 0;
            for (x = firstColumn; x < (firstColumn + nbr_of_columns); x++)
            {
               switch (line[c])
               {
               case ' ':
                  grid[x][y + 1].roleType = GROUND;
                  break;
               case '#':
                  grid[x][y + 1].roleType = WALL;
                  break;
               case '$':
                  grid[x][y + 1].roleType = BOX;
                  break;
               case '*':
                  grid[x][y + 1].roleType = BOX_OK;
                  break;
               case '.':
                  grid[x][y + 1].roleType = GOAL;
                  break;
               case '@':
                  grid[x][y + 1].roleType = PLAYER;
                  break;
               case '+':
                  grid[x][y + 1].roleType = PLAYER;
                  break;
               }
               c++;
            }

         }
      }
   }

   /* free memory */
   xmlFreeDoc (doc);
   xmlXPathFreeObject (xpathLevel);
   xmlXPathFreeContext (ctxt);
   return 1;
}
#endif
