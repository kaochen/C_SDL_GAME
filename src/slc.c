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

/*This file handles the reading and decoding of .slc files.*/

#ifndef SLC_C
#define SLC_C

#include <libintl.h>
#include <locale.h>

#include "slc.h"

/*Manage slc files ==================================================*/
/* Initiatlize the list of files */
S_FilesList *
initFilesList ()
{
  S_FilesList *filesList = malloc (sizeof (S_FilesList));

  if (filesList == NULL)
    {
      fprintf (stderr, "Init files list failed\n");
      exit (EXIT_FAILURE);
    }
  filesList->first = NULL;
  filesList->last = NULL;
  filesList->nbr_of_files = 0;
  return filesList;
}

/*Add the first file in a empty list*/
int
addFirstFile (S_FilesList * filesList, char *name)
{
  S_Files *firstFiles = malloc (sizeof (*firstFiles));
  if (filesList == NULL || firstFiles == NULL)
    {
      fprintf (stderr, "Init first file failed\n");
      return EXIT_FAILURE;
    }
     firstFiles->name = malloc(MAX_CARACT);
   if (firstFiles->name  == NULL )
      {
         fprintf (stderr, "Add new file failed\n");
         exit(EXIT_FAILURE);
      }



  /*update first file data */
  strcpy (firstFiles->name, name);
  firstFiles->previous = NULL;
  firstFiles->next = NULL;

  /*update list files */
  filesList->first = firstFiles;
  filesList->last = firstFiles;
  filesList->nbr_of_files = 1;

  return EXIT_SUCCESS;
}


/*Add a new file in the list*/
void
addNewFile (S_FilesList * filesList, char *name)
{
  S_Files *new = malloc (sizeof (*new));
  if (filesList == NULL || new == NULL)
    {
      exit (EXIT_FAILURE);
    }

   new->name = malloc(MAX_CARACT);
   if (new->name  == NULL )
      {
         fprintf (stderr, "Add new file failed\n");
         exit(EXIT_FAILURE);
      }


  strcpy (new->name, name);

  /* insert at the end of the chain */
  new->next = NULL;
  new->previous = filesList->last;
  filesList->last->next = new;
  filesList->last = new;
  filesList->nbr_of_files++;
}

/*Erase a file in the list from last*/
int
delFile (S_FilesList * filesList)
{
  if (filesList->nbr_of_files == 0)
    return -1;
  S_Files *del_file = malloc (sizeof (S_Files));
   if(del_file == NULL)
        {
         fprintf(stderr,gettext("not enough memory"));
         exit(EXIT_FAILURE);
        }

  del_file = filesList->last;
  filesList->last = filesList->last->previous;
  if (filesList->nbr_of_files == 1)
    filesList->last = NULL;

  free(del_file->name);
  free (del_file);
   if (filesList->nbr_of_files > 0)
     {
      filesList->nbr_of_files--;
      }
  return 0;
}

/* destroy file list */
void
destroyFileList (S_FilesList * filesList)
{

  while (filesList->nbr_of_files > 0)
    {
      delFile (filesList);
    }
  vbPrintf ("\tFiles list destroyed\n");
}

/*Load slc level into the grid */
int
listSlcLevelFiles (S_FilesList * filesList)
{
  DIR *rep;
  rep = opendir (pref.levelsPath);
  struct dirent *file = NULL;
  if (rep == NULL)
    {
      return (EXIT_FAILURE);
    }

  /*search for slc files */
  char path[MAX_CARACT] = "";
  while ((file = readdir (rep)) != NULL)
    {
      if (strstr (file->d_name, ".slc") != NULL
	  || strstr (file->d_name, ".SLC") != NULL)
	{
	  sprintf (path, "%s%s", pref.levelsPath, file->d_name);
	  if (filesList->nbr_of_files == 0)
	    {
	      addFirstFile (filesList, path);
	    }
	  else
	    {
	      addNewFile (filesList, path);
	    }
	  vbPrintf ("Found file %d : %s\n", filesList->nbr_of_files, path);
	}
    }

  if (closedir (rep) == EXIT_FAILURE)
    {
      return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;
}

/* Read the files list one element by one */
int
readFilesList (S_FilesList * filesList)
{
  if (filesList == NULL)
    {
      return EXIT_FAILURE;
    }
  vbPrintf ("Read Files List: \n");
  S_Files *actual = filesList->first;
  vbPrintf ("The file list contain %d files\n",
	   filesList->nbr_of_files);

  /*Read file one by one until you'll get a NULL */
  int i = 1;
  while (actual != NULL)
    {
      vbPrintf ("File %d : %s\n", i, actual->name);
      actual = actual->next;
      i++;
    }
  return EXIT_SUCCESS;
}

/*Manage levels ==================================================*/
/* Initialize the list of levels */
S_LevelList *
initLevelList ()
{
  S_LevelList *levelList = malloc (sizeof (*levelList));
  if (levelList == NULL)
    {
      exit (EXIT_FAILURE);
    }

  levelList->first = NULL;
  levelList->last = NULL;
  levelList->nbr_of_levels = 0;

  return levelList;
}

/*Add first level into an empty list */
int
addFirstLevel (S_LevelList * levelList, char *fileName, char *name, char *author,
	       int height, int width)
{
  S_Level *firstLevel = malloc (sizeof (S_Level));
  if (firstLevel == NULL)
    {
      fprintf (stderr, "Add first level failed\n");
      return EXIT_FAILURE;
    }
        firstLevel->name = malloc(MAX_CARACT);
        firstLevel->fileName = malloc(MAX_CARACT);
        firstLevel->author = malloc(MAX_CARACT);

           if (firstLevel->name == NULL || firstLevel->fileName == NULL  )
             {
               fprintf (stderr, "Add first level failed\n");
               exit(EXIT_FAILURE);
             }
  /*update first file data */
  strcpy (firstLevel->name, name);
  strcpy (firstLevel->fileName, fileName);
  strcpy (firstLevel->author, author);
  firstLevel->height = height;
  firstLevel->width = width;

  firstLevel->previous = levelList->first;
  firstLevel->next = levelList->last;

  /*update  levelList */
  levelList->first = firstLevel;
  levelList->last = firstLevel;
  levelList->nbr_of_levels = 1;
  return EXIT_SUCCESS;
}

/*Add a level in the list*/
void
addNewLevel (S_LevelList * levelList,
             char *fileName,
             char *name,
             char *author,
             int height,
             int width)
{
  S_Level *new = malloc (sizeof (S_Level));
  if (levelList == NULL || new == NULL)
    {
      fprintf (stderr, "Add new level failed\n");
      exit (EXIT_FAILURE);
    }
  /* check size before add */
  if (width <= pref.x_Blocks && height <= (pref.y_Blocks - pref.h_menu_block))
    {
        new->name = malloc(MAX_CARACT);
        new->fileName = malloc(MAX_CARACT);
        new->author = malloc(MAX_CARACT);

           if (new->name == NULL || new->fileName == NULL  )
             {
               fprintf (stderr, "Add first level failed\n");
               exit(EXIT_FAILURE);
             }

      strcpy (new->name, name);
      strcpy (new->fileName, fileName);
      strcpy (new->author, author);
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

/*Erase a level in the list from last*/
int
delLevel (S_LevelList * levelList)
{
  if (levelList->nbr_of_levels == 0)
    {
      levelList->last = NULL;
      return -1;
    }

  S_Level *del_level = malloc (sizeof (S_Level));
  if(del_level == NULL)
        {
         fprintf(stderr,gettext("not enough memory"));
         exit(EXIT_FAILURE);
        }
  del_level = levelList->last;
  levelList->last = levelList->last->previous;

  free(del_level->name);
  free(del_level->fileName);
  free (del_level);

  if (levelList->nbr_of_levels > 0){
   levelList->nbr_of_levels--;
  }
  return 0;
}

/* destroy level list */
void
destroy (S_LevelList * levelList)
{
  while (levelList->nbr_of_levels > 0)
    {
      delLevel (levelList);
    }
  vbPrintf ("\tLevel list destroyed\n");
}

/*get levels infos from files */
int
getNbrOfLevels (S_LevelList * levelList)
{
  int i = 0;
  S_Level *actual = malloc (sizeof (S_Level));
  if (levelList == NULL || actual == NULL)
    {
      perror ("getNbrOfLevels");
      exit (EXIT_FAILURE);
    }
  actual = levelList->first;
  while (actual != NULL)
    {
      i++;
      actual = actual->next;
    }
  //fprintf(stderr, "Found %d levels\n", i);
  return i;
}

/*get levels infos from files */
int
readLevelsAttributs (S_FilesList * filesList,
                     S_LevelList * levelList)
{
  S_Files *actualFile  = malloc (sizeof (*actualFile));
  if (filesList == NULL || levelList == NULL || actualFile == NULL)
    {
      fprintf (stderr, "readLevelsAttributs failed\n");
      return EXIT_FAILURE;
    }

  /*Get files names */
   xmlDoc *doc = NULL;
   xmlNode *cur = NULL;

   LIBXML_TEST_VERSION

  actualFile = filesList->first;
  while (actualFile != NULL)
    {

            /* Read each level from each files */
            vbPrintf ("Read levels from : %s\n", actualFile->name);

            /* Open SLC/XML file */
            doc = xmlParseFile (actualFile->name);
            if (doc == NULL)
	            {
	              fprintf (stderr, "%s not valid\n", actualFile->name);
	              return EXIT_FAILURE;
	            }
           cur = xmlDocGetRootElement(doc);

          if(cur == NULL){
            fprintf(stderr,"%s is empty. \n", actualFile->name);
            xmlFreeDoc(doc);
            return 0;
            }
          char typeRootNodeName[MAX_CARACT] = "SokobanLevels";
          if(xmlStrcmp(cur->name,(const xmlChar *) typeRootNodeName)) {
            fprintf(stderr,"%s is a the wrong type, root node is not %s  \n", actualFile->name, typeRootNodeName);
            xmlFreeDoc(doc);
            return 0;
            }

            // Start XPath
            xmlXPathInit ();
            // Create a context
            xmlXPathContextPtr ctxt = xmlXPathNewContext (doc);
            if (ctxt == NULL)
	            {
	              fprintf (stderr, "Error creating the context XPath\n");
	              return EXIT_FAILURE;
	            }

            /* Read Level */
            xmlXPathObjectPtr xpathLevel =
	          xmlXPathEvalExpression (BAD_CAST
				             "/SokobanLevels/LevelCollection/Level", ctxt);
            if (xpathLevel == NULL)
	            {
	              fprintf (stderr, "Error on the xPathLevel expression\n");
	              return EXIT_FAILURE;
	            }


            /*get copyright */
            char copyright[MAX_CARACT] = "";
            getAttributFromXML(cur, copyright, "LevelCollection", "Copyright" );

            /*get specific attributs from each level*/
            xmlChar *name = NULL;
            xmlChar *width = NULL;
            xmlChar *height = NULL;

            /*Get the number of levels in a file */
            int levelCount = 0, i = 0;
            levelCount = xpathLevel->nodesetval->nodeNr;
            vbPrintf ( "The files %s contain %d levels\n\n",
	             actualFile->name, levelCount);
            /*Add S_Level for each levels found */
            while (i < levelCount)
	            {
	              cur = xpathLevel->nodesetval->nodeTab[i];
	              for (xmlAttrPtr attr = cur->properties; NULL != attr;
	                   attr = attr->next)
	                {
	                  name = xmlGetProp (cur, (const xmlChar *)"Id");
	                  width = xmlGetProp (cur, (const xmlChar *)"Width");
	                  height = xmlGetProp (cur, (const xmlChar *)"Height");
	                }
	              /*Load infos into the levelList */
	              if (levelList->nbr_of_levels == 0)
	                {
	                  addFirstLevel (levelList, actualFile->name, (char *) name, copyright,
			                 atoi ((char *) height), atoi ((char *) width));
	                }
	              else
	                {
	                  addNewLevel (levelList, actualFile->name, (char *) name, copyright,
			               atoi ((char *) height), atoi ((char *) width));
	                }
	              //printf ("File: %s, name: %s, width: %s, height: %s, Copyright: %s\n",actualFile->name, name, width, height, copyright);
	              i++;
	            }
            /* free memory */
            xmlXPathFreeContext (ctxt);
            xmlFreeDoc (doc);
            xmlFree(name);
            xmlFree(width);
            xmlFree(height);
            actualFile = actualFile->next;
          }
  free(actualFile);
  return EXIT_SUCCESS;
}

/* read level list one by one */
void
readLevelList (S_LevelList * levelList)
{
  if (levelList == NULL)
    {
      exit (EXIT_FAILURE);
    }
  S_Level *actual = levelList->first;
  while (actual != NULL)
    {
      vbPrintf ("%s, %d:%d\n", actual->name, actual->width,
	       actual->height);
      actual = actual->next;
    }
}

/*Load slc level into the grid */
int
loadSlcLevel (S_LevelList * levelList,
              Square grid[pref.max_X_Blocks][pref.max_Y_Blocks],
              S_Menu gridMenu[pref.max_X_Blocks][pref.max_Y_Blocks])
{
  if (levelList == NULL)
    {
      return EXIT_FAILURE;
    }
  int nbr_of_lines = 0, firstLines = 0, nbr_of_columns = 0, firstColumn =
    0, i = 0;
  S_Level *actual = levelList->first;
  /* read the all chain list */
  while (actual != NULL)
    {
      /* try to find the nameLevel into the list */
      if (i == pref.level)
	{
	  vbPrintf ("Found %s, %d:%d\n", actual->name,
		   actual->width, actual->height);
	  nbr_of_lines = actual->height;
	  nbr_of_columns = actual->width;
    ///Update menu icon if the level is already achieved
    pref.achieved = testIfLevelAchieved(actual->name, gridMenu);
    vbPrintf ("pref.achieved %d\n", pref.achieved);

	  /*Place the drawing into the center of the grid */
	  firstLines = ((pref.y_Blocks - nbr_of_lines) / 2 + pref.h_menu_block);	// + menu
	  firstColumn = (pref.x_Blocks - nbr_of_columns) / 2;

	  break;
	}
      i++;
      actual = actual->next;
    }

  xmlDocPtr doc;

  /* Open SLC/XML file */
  doc = xmlParseFile (actual->fileName);
  if (doc == NULL)
    {
      fprintf (stderr, "XML Document not valid\n");
      return EXIT_FAILURE;
    }
  // Start XPath
  xmlXPathInit ();
  // Create a context
  xmlXPathContextPtr ctxt = xmlXPathNewContext (doc);
  if (ctxt == NULL)
    {
      fprintf (stderr, "Error creating the context XPath\n");
      return EXIT_FAILURE;
    }

  /* Read Level */
  char path[MAX_CARACT] = "";
  sprintf (path,
	   "/SokobanLevels/LevelCollection/Level[@Id=\"%s\"]/L/text()",
	   actual->name);
  xmlXPathObjectPtr xpathLevel = xmlXPathEvalExpression (BAD_CAST path, ctxt);
  if (xpathLevel == NULL)
    {
      fprintf (stderr, "Error on the xPathLevel expression\n");
      return EXIT_FAILURE;
    }
  /*Clean grid before */
  int y = 0, x = 0;
  for (y = 0; y < pref.y_Blocks; y++)
    {
      for (x = 0; x < pref.x_Blocks; x++)
	{
	  grid[x][y].mainRole = GROUND;
	  grid[x][y].subRole = GROUND1;
	  grid[x][y].goalRole = EMPTY;
	  grid[x][y].playerRole = EMPTY;
	}
    }
  /* load level into the grid */
  if (xpathLevel->type == XPATH_NODESET)
    {
      int c = 0;
      y = 0, x = 0;
      xmlNodePtr n;
      for (y = 0; y < nbr_of_lines; y++)
	{
	  n = xpathLevel->nodesetval->nodeTab[y];
	  if (n->type == XML_TEXT_NODE || n->type == XML_CDATA_SECTION_NODE)
	    {
        char line[MAX_CARACT] = "";
	      strcpy (line, (char *) n->content);
	      fprintf (stderr, "%s\n", n->content);
	      /* load level into the grid */
	      c = 0;
	      for (x = firstColumn; x < (firstColumn + nbr_of_columns); x++)
		{
		  int y2 = y + firstLines;

		  switch (line[c])
		    {
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
		      grid[x][y2].goalRole = GOAL;
		      break;
		    case '.':
		      grid[x][y2].mainRole = GROUND;
		      grid[x][y2].goalRole = GOAL;
		      break;
		    case '@':
		      grid[x][y2].mainRole = PLAYER;
		      grid[x][y2].playerRole = PLAYER_F;
		      break;
		    case '+':
		      grid[x][y2].mainRole = PLAYER;
		      grid[x][y2].playerRole = PLAYER_F;
		      break;
		    }
		  c++;
		}
	    }
	}
    }

  randomGround (grid);
  randomWall (grid);
  /* Change grounds that are outside the walls to the OUTSIDE flag */
  detectOutside (grid);
  randomOutside (grid);


  /* free memory */
  xmlFreeDoc (doc);
  xmlXPathFreeContext (ctxt);
  xmlXPathFreeObject(xpathLevel);

  /*refresh status*/
  menuChoice.freeze = 0;
  menuChoice.open = 0;
  fprintf (stderr, "Level %d loaded\n", (pref.level + 1));


  return EXIT_SUCCESS;
}

/*get a specific attribut from slc file*/
int
getAttributFromXML(const xmlNode *cur, char *value, const char *nodeName, const char *attributName )
{
        xmlChar *buf = NULL;
        cur = cur->xmlChildrenNode;
        while (cur != NULL)
          {
            if ((!xmlStrcmp(cur->name,(const xmlChar *)nodeName)))
              {
                buf = xmlGetProp(cur,(const xmlChar *)attributName);
                //vbPrintf("Copyright: %s\n", buf);
                sprintf(value,"%s", buf);
              }
            cur = cur->next;
          }
          xmlFree(buf);
  return 1;
}


#endif
