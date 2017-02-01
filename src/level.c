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

/*This file handles the basic functions needed to build and display the level.*/

#ifndef LEVEL_C
#define LEVEL_C

#include "../inc/level.h"

#include <libintl.h>
#include <locale.h>

/* Init grid */
void
grid_init(Square grid[pref.max_X_Blocks][pref.max_Y_Blocks])
{
   for(int y=0; y < pref.max_Y_Blocks; ++y)
      for(int x=0; x< pref.max_X_Blocks; ++x)
      grid[x][y] = (Square) {.mainRole=GROUND, .subRole=EMPTY, .goalRole=EMPTY, .boxRole=EMPTY, .playerRole=EMPTY, .target=EMPTY };
}



/* display level on the screen */
int
displayLevel (Square grid[pref.max_X_Blocks][pref.max_Y_Blocks],
              SDL_Surface * screen,
              Sprites tableSurface[NBR_OF_IMAGES])
{
 if (grid == NULL || screen == NULL || tableSurface == NULL)
    {
      fprintf (stderr, gettext("init displayLevel failed: %s\n"),
	       SDL_GetError ());
      return EXIT_FAILURE;
    }

  /* set a goal position */
  SDL_Rect pos;

  /* blit surfaces depending of its destiny */
  int x = 0, y = 0;
  /*first pass */
  for (x = 0; x < pref.x_Blocks; x++)
    {
      for (y = 0; y < pref.y_Blocks; y++)
	{
	  pos.x = x * SPRITE_SIZE;
	  pos.y = y * SPRITE_SIZE;

	  /* blit all blocks depending on grid subRole */
	  switch (grid[x][y].subRole)
	    {
	    case GROUND1:
	      SDL_BlitSurface (tableSurface[GROUND1_IMAGE].image, NULL,
			       screen, &pos);
	      break;
	    case GROUND2:
	      SDL_BlitSurface (tableSurface[GROUND2_IMAGE].image, NULL,
			       screen, &pos);
	      break;
	    case GROUND3:
	      SDL_BlitSurface (tableSurface[GROUND3_IMAGE].image, NULL,
			       screen, &pos);
	      break;
	    case OUTSIDE:
	      SDL_BlitSurface (tableSurface[OUTSIDE_IMAGE].image, NULL,
			       screen, &pos);
	      break;
	    case OUTSIDE2:
	      SDL_BlitSurface (tableSurface[OUTSIDE2_IMAGE].image, NULL,
			       screen, &pos);
	      break;
	    case OUTSIDE3:
	      SDL_BlitSurface (tableSurface[OUTSIDE3_IMAGE].image, NULL,
			       screen, &pos);
	      break;
	    case OUTSIDE4:
	      SDL_BlitSurface (tableSurface[OUTSIDE4_IMAGE].image, NULL,
			       screen, &pos);
	      break;
	    case WALL1:
	      SDL_BlitSurface (tableSurface[WALL1_IMAGE].image, NULL, screen,
			       &pos);
	      break;
	    case WALL2:
	      SDL_BlitSurface (tableSurface[WALL2_IMAGE].image, NULL, screen,
			       &pos);
	      break;
	    case WALL3:
	      SDL_BlitSurface (tableSurface[WALL3_IMAGE].image, NULL, screen,
			       &pos);
	      break;
	    case WALL4:
	      SDL_BlitSurface (tableSurface[WALL4_IMAGE].image, NULL, screen,
			       &pos);
	      break;
	    }

	  switch (grid[x][y].goalRole)
	    {
	    case GOAL:
	      SDL_BlitSurface (tableSurface[GOAL_IMAGE].image, NULL, screen,
			       &pos);
	      break;
	    }

	  /* blit all blocks depending on grid mainRole */
	  switch (grid[x][y].mainRole)
	    {
	    case BOX:
	      if (grid[x][y].goalRole == GOAL)
		{
		  SDL_BlitSurface (tableSurface[BOX_IMAGE_OK].image, NULL,
				   screen, &pos);
		}
	      else
		{
		  SDL_BlitSurface (tableSurface[BOX_IMAGE].image, NULL,
				   screen, &pos);
		}
	      break;
	    case PLAYER:
	      SDL_BlitSurface (tableSurface[GROUND1_IMAGE].image, NULL,
			       screen, &pos);
	      break;
	    }

	  switch (grid[x][y].playerRole)
	    {
	    case PLAYER_F:
	      SDL_BlitSurface (tableSurface[PLAYER_FRONT].image, NULL, screen,
			       &pos);
	      break;
	    case PLAYER_B:
	      SDL_BlitSurface (tableSurface[PLAYER_BACK].image, NULL, screen,
			       &pos);
	      break;
	    case PLAYER_L:
	      SDL_BlitSurface (tableSurface[PLAYER_LEFT].image, NULL, screen,
			       &pos);
	      break;
	    case PLAYER_R:
	      SDL_BlitSurface (tableSurface[PLAYER_RIGHT].image, NULL, screen,
			       &pos);
	      break;
	    }

        /*display target over ground */
        switch (grid[x][y].target)
	    {
	    case TARGET:
	      SDL_BlitSurface (tableSurface[TARGET_IMAGE].image, NULL, screen,
			       &pos);
          grid[x][y].target = EMPTY;
	      break;
        }

	}

    }
  if (blitBorders (grid, screen, tableSurface) == EXIT_FAILURE)
    {
         fprintf (stderr, gettext("blitBorders failed.\n"));
         return EXIT_FAILURE;
    }

   if (blitCorners (grid, screen, tableSurface) == EXIT_FAILURE)
    {
         fprintf (stderr, gettext("blitCorners failed.\n"));
         return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}



/*get level name*/
int
getLevelName (S_LevelList * levelList, char *levelName)
{
  if (levelList == NULL)
    {
      return EXIT_FAILURE;
    }
  int i = 0;
  S_Level *actual = levelList->first;
  /* read the all chain list */
  while (actual != NULL)
    {
      /* try to find the nameLevel into the list */
      if (i == pref.level)
	{
	  //fprintf (stderr, "Actual name Level \"%s\"\n", actual->name);
	  strcpy (levelName, actual->name);
	  break;
	}
      i++;
      actual = actual->next;
    }
  return EXIT_SUCCESS;
}

/*get level current filename*/
int
getFileName (S_LevelList * levelList, char *fileName)
{
  if (levelList == NULL)
    {
      return EXIT_FAILURE;
    }
  int i = 0;
  S_Level *actual = levelList->first;
  /* read the all chain list */
  while (actual != NULL)
    {
      /* try to find the nameLevel into the list */
      if (i == pref.level)
	{
	  //fprintf (stderr, "Actual name Level \"%s\"\n", actual->fileName);
	  strcpy (fileName, actual->fileName);
	  break;
	}
      i++;
      actual = actual->next;
    }
  return EXIT_SUCCESS;
}


/*get level current author*/
int
getAuthor (S_LevelList * levelList, char *author)
{
  if (levelList == NULL)
    {
      return EXIT_FAILURE;
    }
  int i = 0;
  S_Level *actual = levelList->first;
  /* read the all chain list */
  while (actual != NULL)
    {
      /* try to find the nameLevel into the list */
      if (i == pref.level)
	{
	  //fprintf (stderr, "Actual Author \"%s\"\n", actual->author);
	  strcpy (author, actual->author);
	  break;
	}
      i++;
      actual = actual->next;
    }
  return EXIT_SUCCESS;
}

/*Trunk long char*/
int
trunkLongChar(size_t size, char * text )
{
//  fprintf (stderr, "IN : %s\n", text);
	if (strlen (text) > size)
		{
     char buf[MAX_CARACT]="";
     strncat(buf,text,size);
		 sprintf (text, "%s...", buf);
		}
//  fprintf (stderr, "OUT: %s\n", text);
  return 1;
}

/*Found the level number with just the name*/
int
findLevelNumber (S_LevelList * levelList, char *levelName)
{
  if (levelList == NULL)
    {
      fprintf (stderr, "struct levelList does not exit");
    }
  int i = 0;
  S_Level *actual = levelList->first;
  /* read the all chain list */
  while (actual != NULL)
    {
      /* try to find the nameLevel into the list */
      if (!strcmp (actual->name, levelName))
	{
	  fprintf (stderr, "I found the level %s : %d\n", actual->name, i);
	  return i;
	  break;
	}
      else
	{
	  i++;
	  actual = actual->next;
	}
    }
  return 0;
}

/*get a levelchoice by reading the level name from the preference file*/
int
readLevelFromSetting (S_LevelList * levelList)
{
  int i = 0, ret = 0;
  char levelName[MAX_CARACT] = "";
  getPrefChar (levelName, "LevelName");
  fprintf (stderr, "First level to load is %s\n", levelName);

  if (levelList == NULL)
    {
      exit (EXIT_FAILURE);
    }
  S_Level *actual = levelList->first;
  while (actual != NULL)
    {
      if (strstr (actual->name, levelName) != NULL)
	{
	  ret = i;
	  break;
	}
      else
	{
	  i++;
	}
      actual = actual->next;
    }
  fprintf (stderr, "levelchoice from file : %d\n ", ret);
  return ret;
}

/*force reload of a level*/
int
changeLevel(int i)
{
		  pref.level += i;
      pref.reload = 1;
  return 1; //refresh
}
#endif
