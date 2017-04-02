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
#define SETTINGS_C

#include "settings.h"
#include "menu.h"
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>

/* Setup all paths */
void
setupPath(char * argv0){

FILE *file = NULL;
char path[MAX_PATH]= "";
char *pLastWord = NULL;
pLastWord = strrchr(argv0, '/');
size_t l = strlen(argv0) - strlen(pLastWord) + 1;
printf("lastword : %s\n", pLastWord);
strncat(path, argv0,l);

// setup all folders paths
strcpy (pref.pgmPath, path); // root program path
sprintf (pref.imgPath, "%s"IMG_FOLDER"",pref.pgmPath);
sprintf (pref.levelsPath, "%s"LEVELS_FOLDER"",pref.pgmPath);
setupSessionFilePath();

// check if PREF_FILE exist
strcat(path,PREF_FILE);
file = fopen(path, "r");

if (file != NULL){
    strcpy(argv0,path);
    fprintf (stderr, "found %s\n", path);
    strcpy(pref.iniPath, path);
    }
else{
      fprintf (stderr, "Error opening %s :: %s\n", path, strerror (errno));
      exit(EXIT_FAILURE);
    }
fclose(file);
}

//Try to find a user place to store SESSION_FILE
void
setupSessionFilePath(){
  struct passwd *pwd = getpwuid(getuid());
  char path[MAX_PATH];
  if(pwd){
    sprintf (path, "%s/.local/share/"GAME_NAME"/",pwd->pw_dir);
  }
  else{
    sprintf (path, "%s/.local/share/"GAME_NAME"/",getenv("HOME"));
  }
  fprintf (stderr,"Session file will be at %s\n", pref.sessionPath);

  //place SESSION_FILE into the user folder.
  // Create a sub folder if necessary
  struct stat st = {0};
  if (stat(path, &st) == -1){
    mkdir(path, 0755);
  }

  // Create SESSION_FILE if necessary
  sprintf (pref.sessionPath, "%s"SESSION_FILE"",path);

  FILE *sessionFile;
  sessionFile = fopen(pref.sessionPath, "rb+");
  if (sessionFile == NULL)
    {
      sessionFile = fopen(pref.sessionPath, "wb");
    }
  fclose(sessionFile);

}


/* load settings from pref file */
int
loadPrefStruct(SDL_DisplayMode current){

  int i = 0;
  /*get infos from the current display */
  for(i = 0; i < SDL_GetNumVideoDisplays(); ++i){
    int should_be_zero = SDL_GetCurrentDisplayMode(i, &current);
    if(should_be_zero != 0)
      // In case of error...
      SDL_Log("Could not get display mode for video display #%d: %s", i, SDL_GetError());

    else
      // On success, print the current display mode.
      SDL_Log("Display #%d: current display mode is %dx%dpx @ %dhz.", i, current.w, current.h, current.refresh_rate);
   }


   /* Main window */
      /* Window size */
      pref.display_width = current.w;
      pref.display_height = current.h;

      pref.window_width = getWindow_width();
      pref.window_height = getWindow_height();
   /* Number of blocks */
      pref.x_Blocks = pref.window_width/SPRITE_SIZE;
      pref.y_Blocks = pref.window_height/SPRITE_SIZE;
      pref.max_X_Blocks =  pref.x_Blocks + 2;
      pref.max_Y_Blocks =  pref.y_Blocks + 2;

   /* Menu */
      pref.x_menu = (pref.window_width - MENU_WIDTH )/2;
      pref.xb_menu = pref.x_menu/SPRITE_SIZE;
      pref.y_menu = 0;
      pref.h_menu_block = 1;

   /* Level */
      pref.level = 0;
      pref.level_max = 1;
      pref.reload = 0;

   /*framerate*/
      pref.display_framerate = current.refresh_rate;
      pref.framerate = getWindow_framerate();

    /*Theme*/
    getThemePath();

  return 1;
}

/*get int value from a ini file */
int
readInt (const char *fileName, const char *prefName)
{
  FILE *prefFile = NULL;
  char line[MAX_CARACT] = "";
  char settingName[MAX_CARACT] = "";
  strcpy (settingName, prefName);

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
      fprintf (stderr, "Error opening %s: %s\n", pref.iniPath, strerror (errno));
      exit(EXIT_FAILURE);
    }
  fclose (prefFile);
  return ret;
}

void
readChar(const char *fileName, char * pref, const char *setting) {

    char line[MAX_CARACT] = "";
    char ret[MAX_CARACT] = "";
    char ret2[MAX_CARACT] = "";
    char *test = NULL;
    strcpy(ret, pref);
    unsigned int i = 0 , size = 0;
    int j = -1;

    FILE *prefFile = NULL;
    prefFile = fopen (fileName, "r");
    if (prefFile == NULL)
         {
            fprintf (stderr, "Error opening %s: %s\n", fileName, strerror (errno));
            strcpy(pref, "");
         }
      else
        {
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

                       /*dump quotation marks "" */
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
}

//look at the session file if the current level has not been already achieved
int
testIfLevelAchieved(const char *levelName,
                    S_Menu gridMenu[pref.max_X_Blocks][pref.max_Y_Blocks])
  {
      char lineName[MAX_CARACT]= "Finished";
      char line[MAX_CARACT]="";

      FILE *file = NULL;
      file = fopen (pref.sessionPath, "r");

      int i = 0;
      if (file != NULL)
        {
          while (fgets (line, MAX_CARACT, file) != NULL)
	          {
	            if (strstr (line, levelName) != NULL  && strstr (line, lineName) != NULL)
                {
                i++;
                }
            }
            if (i != 0)
              {
                fprintf (stderr, "You have already succeed to finish this level\n");
                gridMenu[pref.xb_menu + 9][0] = (S_Menu) {.role=M_CHECKED, .type=TOPBAR, .tab=0, .image=BUTTON_CHECKED};
                return 1;
	            }
        }
      else
        {
          fprintf (stderr, "Error opening %s: %s\n", pref.sessionPath, strerror (errno));
          return EXIT_FAILURE;
        }
  fclose (file);
  fprintf (stderr, "You have never finished this level\n");
  gridMenu[pref.xb_menu + 9][0] = (S_Menu) {.role=M_CHECK, .type=TOPBAR, .tab=0, .image=BUTTON_CHECK};
  return 0;
}

int
getWindow_width (void)
{
  int ret = readInt (pref.iniPath, "window_width");
  if (ret < W_WIDTH)
    {
      ret = W_WIDTH;
    }
  if (ret > pref.display_width){
    ret = pref.display_width - 80;
  }
  return ret;
}

int
getWindow_height (void)
{
  int ret = readInt (pref.iniPath, "window_height");
  if (ret < W_HEIGHT)
    {
      ret = W_HEIGHT;
    }
  if (ret > pref.display_height){
    ret = pref.display_height - 80;
  }
  return ret;
}

int
getWindow_framerate (void)
{
       int fps = readInt (pref.iniPath,"framerate");
        if (fps < 12){
            fps = 12;
        }
        if (fps > pref.display_framerate){
          fps = pref.display_framerate;
        }
        return fps;
}



/* write a pref char */
int
writeChar (const char *fileName ,const char *lineName, const char *value)
{
  char line[MAX_CARACT] = "";

  vbPrintf ("add to %s file : %s = %s\n", fileName, lineName, value);

  FILE *prefFile = NULL;
  prefFile = fopen (fileName, "r+");
  if (prefFile == NULL)
    {
      prefFile = fopen (fileName, "w+");
    }

  FILE *tmpFile = NULL;
  tmpFile = fopen ("tmp.ini", "w+");
  int i = 0;
  if (prefFile != NULL && tmpFile != NULL)
    {
      while (fgets (line, MAX_CARACT, prefFile) != NULL)
	{
	  if (strstr (line, lineName) != NULL)
	    {
	      fprintf (tmpFile, "%s = \"%s\"\n", lineName, value);
        i++;
	      strcpy (line, "");	//empty the line buffer
	    }
	  else
	    {
	      fprintf (tmpFile, "%s", line);
	    }
	}
      if(i == 0)
        {
	      fprintf (tmpFile, "%s = \"%s\"\n", lineName, value);
        }
    }
  else
    {
      fprintf (stderr, "Error opening %s: %s\n", fileName, strerror (errno));
      return EXIT_FAILURE;
    }
  fclose (prefFile);
  fclose (tmpFile);
  rename ("tmp.ini", fileName);

  return EXIT_SUCCESS;
}

/*get the theme Path*/
int
getThemePath(void)
{
 /*read theme choice */
  vbPrintf ("Loading Theme:\n");
  char themePath[MAX_CARACT] = "";
  char bufPath[MAX_CARACT] = "";
  readChar (pref.iniPath, bufPath, "theme");

  sprintf (pref.themeName, "%s", bufPath);
  sprintf (themePath, "%s"IMG_FOLDER"%s/",pref.pgmPath, bufPath);
  vbPrintf ("Theme: %s\n", themePath);

  /*test if folder exist */
  struct stat file_stat;
  if (stat (themePath, &file_stat) < 0)
    {
      printf ("The theme folder %s does not exist\n", themePath);
      strcpy (themePath, ""IMG_FOLDER"original/");
      fprintf (stderr, "Change theme folder to default: %s\n\n", themePath);
    }
  else
    {
      vbPrintf ("The theme folder %s exist\n\n", themePath);
    }
  strcpy(pref.themePath, themePath);

  return EXIT_SUCCESS;
}

// print only if verbose mode is on.
void
vbPrintf(const char *format, ...){

  va_list args;
  va_start(args, format);
  if (pref.verbosity == true)
    {
      vfprintf(stderr, format, args);
    }
}

//store the names of the levels already achieved
int
storeLevelsFinished(const char *fileName, const char *levelName)
{
  char line[MAX_CARACT] = "";
  char lineName[MAX_CARACT] = "Finished";

  FILE *file = NULL;
  file = fopen (fileName, "r+");
  if (file == NULL)
    {
      file = fopen (fileName, "w+");
    }

  int i = 0;
  if (file != NULL)
    {
      while (fgets (line, MAX_CARACT, file) != NULL)
	      {
	        if (strstr (line, levelName) != NULL  && strstr (line, lineName) != NULL)
              {
              i++;
              }
        }
        if (i == 0)
            {
  	          fprintf (file, "%s = \"%s\"\n", lineName, levelName);
            }
	          //strcpy (line, "");	//empty the line buffer
	      }
    else
      {
        fprintf (stderr, "Error opening %s: %s\n", fileName, strerror (errno));
        return EXIT_FAILURE;
      }
  fclose (file);
  return EXIT_SUCCESS;
}

#endif
