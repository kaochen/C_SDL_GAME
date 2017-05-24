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

#ifndef FILES_C
#define FILES_C

#include <libintl.h>
#include <locale.h>

#include "files.h"

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

#endif
