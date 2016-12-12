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
/*This file handles the reading and decoding of the setting file "preference.ini".*/

#ifndef SETTINGS_C
#define SETIINGS_C

#include "../inc/settings.h"
#include "../inc/menu.h"
#include <string.h>
#include <errno.h>

/* Searching for preferences.ini */
void
searchPrefFile(char * prefFile){

FILE *file = NULL;
char path[MAX_CARACT]= "./";
strcat(path,PREF_FILE);
file = fopen(path, "r");

if (file != NULL){
    strcpy(prefFile,path);
    }
else{
      fprintf (stderr, "Error opening %s: %s\n", PREF_FILE, strerror (errno));
      exit(EXIT_FAILURE);
    }
fclose(file);
}

/* load settings from pref file */
S_preferences *
loadPrefStruct(void){

  S_preferences *pref;
  pref = malloc (MENU_LINE * sizeof (S_preferences));
   /* Main window */
      /* Window size */
      pref->window_width = getWindow_width();
      pref->window_height = getWindow_height();
   /* Number of blocks */
      pref->x_Blocks = pref->window_width/SPRITE_SIZE;
      pref->y_Blocks = pref->window_height/SPRITE_SIZE;
      pref->max_X_Blocks =  pref->x_Blocks + 2;
      pref->max_Y_Blocks =  pref->y_Blocks + 2;

   /* Menu */
      pref->x_menu = (pref->window_width - MENU_WIDTH )/2;
      pref->y_menu = 0;
      pref->h_menu_block = 1;

   /* Level */
      pref->level = 0;
      pref->level = 1;

   /*framerate*/
    int fps = getPrefAsInt ("framerate");
        if (fps < 12)
          {
            fps = 12;
          }
      pref->framerate = fps;
  return pref;
}

/*get int value from the preference file */
int
getPrefAsInt (const char *prefName)
{
  FILE *prefFile = NULL;
  char line[MAX_CARACT] = "";
  char settingName[MAX_CARACT] = "";
  strcpy (settingName, prefName);

  char fileName[MAX_CARACT]= "";
  searchPrefFile(fileName);

  int ret = 0;
  prefFile = fopen (fileName, "r");
  if (prefFile != NULL)
    {

      while (fgets (line, MAX_CARACT, prefFile) != NULL)
	{
	  if (strstr (line, settingName) != NULL)
	    {
	      //fprintf(stderr,"line: %s", line);
	      if (strpbrk (line, "0123456789") != NULL)
		{
		  ret = atoi (strpbrk (line, "0123456789"));
		}
	      else
		{
		  ret = 0;
		}
	    }
	}

    }
  else
    {
      fprintf (stderr, "Error opening %s: %s\n", PREF_FILE, strerror (errno));
      exit(EXIT_FAILURE);
    }
  fclose (prefFile);
  return ret;
}

void
getPrefChar(char * pref, const char *setting) {

    FILE *prefFile = NULL;
    char line[MAX_CARACT] = "";
    char ret[MAX_CARACT] = "";
    char ret2[MAX_CARACT] = "";
    char *test = NULL;
    strcpy(ret, pref);
    unsigned int i = 0 , size = 0;
    int j = -1;

    char fileName[MAX_CARACT]= "";
    searchPrefFile(fileName);

    prefFile = fopen (fileName, "r");
      if (prefFile == NULL)
         {
            fprintf (stderr, "Error opening %s: %s\n", PREF_FILE, strerror (errno));
            exit(EXIT_FAILURE);
         }
      while (fgets (line, MAX_CARACT, prefFile) != NULL)
	      {
	        if (strstr (line, setting) != NULL)
	          {
                  if (strchr (line, '"') != NULL)
		               {
		                 test = strchr (line, '"');
                       strcpy (ret, test);
		               }
	                     else
		               {
		                 strcpy (ret, "");
		               }

               /*dump quatation marks "" */
	                  size = strlen (ret);
	                  for (i = 0; i < size; i++)
		                  {
		                    if (ret[i] == '"')
		                      {
		                      }
		                    else
		                      {
		                        j++;
		                        ret2[j] = ret[i];
		                      }
		                  }
                 strcpy (ret, ret2);

	               /*dump \n */
                  for(i = 0; i <= strlen (ret); i++ )
                    {
                    if (ret[i] == '\n')
                      {
                      ret[i] = '\0';
                      }
                     }
               	//fprintf(stderr,"line: %s", ret);
                  strcpy(pref, ret);
             }

         }
      fclose (prefFile);
}


int
getWindow_width ()
{
  int ret = getPrefAsInt ("window_width");
  if (ret < W_WIDTH)
    {
      ret = W_WIDTH;
    }
  return ret;
}

int
getWindow_height ()
{
  int ret = getPrefAsInt ("window_height");
  if (ret < W_HEIGHT)
    {
      ret = W_HEIGHT;
    }
  return ret;
}




/* write a pref char */
int
writePrefChar (const char *prefName, const char *value)
{

  char line[MAX_CARACT] = "";
  char settingName[MAX_CARACT] = "";
  strcpy (settingName, prefName);
  char settingValue[MAX_CARACT] = "";
  strcpy (settingValue, value);

  printf ("searching :%s = %s\n", settingName, value);

  FILE *prefFile = NULL;
  prefFile = fopen (PREF_FILE, "r+");
  FILE *tmpFile = NULL;
  tmpFile = fopen ("tmp.ini", "w+");

  if (prefFile != NULL && prefFile != NULL)
    {

      while (fgets (line, MAX_CARACT, prefFile) != NULL)
	{
	  if (strstr (line, settingName) != NULL)
	    {
	      fprintf (tmpFile, "%s = \"%s\"\n", settingName, value);
	      strcpy (line, "");	//empty the line buffer
	    }
	  else
	    {
	      fprintf (tmpFile, "%s", line);
	    }
	}
    }
  else
    {
      fprintf (stderr, "Error opening %s: %s\n", PREF_FILE, strerror (errno));
      return EXIT_FAILURE;
    }
  fclose (prefFile);
  fclose (tmpFile);
  rename ("tmp.ini", PREF_FILE);

  return EXIT_SUCCESS;
}
#endif
