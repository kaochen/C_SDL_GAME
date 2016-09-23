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

#include <libintl.h>
#include <locale.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../inc/const.h"
#include "../inc/game.h"
#include "../inc/level.h"
#include "../inc/menu.h"
#include "../inc/settings.h"
#include "../inc/sprites.h"
#include "../inc/slc.h"


int
main (int argc, char *argv[])
{
  /* Set up gettext for internationalisation */
  setlocale(LC_ALL, "");
  textdomain("sokorobot");
  bindtextdomain("sokorobot", "/usr/share/locale");

  /*manage argument (not used for now) */
  int i;

  for (i = 1; i < argc; i++)
    {
      printf (gettext("Argument %d : %s \n"), i, argv[i]);
    }

  /* reset errors */
  errno = 0;

  /* start random processor just once */
  srand (time (NULL));

  /* Start and check if SDL start correctly */
  if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_TIMER) == -1)
    {
      fprintf (stderr, gettext("SDL initialization error: %s\n"), SDL_GetError ());
      exit (EXIT_FAILURE);
    }

  /* Start SDL TTF */
  if (TTF_Init () == -1)
    {
      fprintf (stderr, gettext("TTF_Init initialization error: %s\n"),
	       TTF_GetError ());
      exit (EXIT_FAILURE);
    }

  /*load preferences */

  printf (gettext("The window size request from settings is %dx%d\n"), getWindow_width (),
	  getWindow_height ());

  /* Create the window game */
  SDL_Window *window = SDL_CreateWindow (GAME_NAME,
					 SDL_WINDOWPOS_UNDEFINED,
					 SDL_WINDOWPOS_UNDEFINED,
					 getWindow_width (),
					 getWindow_height (),
					 SDL_WINDOW_MOUSE_FOCUS);

  /* Check window */
  if (window == NULL)
    {
      fprintf (stderr, gettext("SDL_CreateWindow failed: %s\n"),
	       SDL_GetError ());
      exit (EXIT_FAILURE);
    }

  /* Set window icon */
  SDL_SetWindowIcon (window, IMG_Load ("img/icon.png"));

  /* Create a surface from the window */
  SDL_Surface *screen = NULL;
  screen = SDL_GetWindowSurface (window);

  /* Check if window and surface are created */
  if (screen == NULL)
    {
      fprintf (stderr, gettext("SDL_GetWindowSurface failed: %s\n"),
	       SDL_GetError ());
      exit (EXIT_FAILURE);
    }

  fprintf (stderr, gettext("The SDL window has been created.\n"));

  fprintf (stderr, "\n");
  /* load images into a table of struct */
  Sprites tableSurface[NBR_OF_IMAGES];

  for (i = 0; i <=NBR_OF_IMAGES; i++)
    {
      tableSurface[i].image = 0;
    }
  loadAllSprites (tableSurface);

  /* create a grid with coordinates x,y to locate Role from the struct Square */
  int maxBlocks = getMax_Blocks ();
  Square grid[maxBlocks][maxBlocks];
  int xGrid = 0;
  int yGrid = 0;
    for(yGrid = 0; yGrid < maxBlocks; yGrid++)
     {
        for(xGrid = 0; xGrid < maxBlocks; xGrid++)
           {
               grid[xGrid][yGrid].mainRole = GROUND;
               grid[xGrid][yGrid].subRole = EMPTY;
               grid[xGrid][yGrid].goalRole = EMPTY;
               grid[xGrid][yGrid].boxRole = EMPTY;
               grid[xGrid][yGrid].playerRole = EMPTY;

           }
     }



  char levelName[MAX_CARACT] = "";

  fprintf (stderr, "\n");
  /*List slc files from the levels/ folder */
  S_FilesList *filesList = initFilesList ();
  if (listSlcLevelFiles (filesList) == EXIT_FAILURE)
    {
      perror (gettext("The level files cannot from the folder levels/ be listed."));
    }

  fprintf (stderr, "\n");
  /*Read files name from the filesList to check */
  if (readFilesList (filesList) == EXIT_FAILURE)
    {
      perror (gettext("The level list cannot be verified."));
    }


  fprintf (stderr, "\n");

  /*Read level from slc file */
  S_LevelList *levelList = initLevelList ();
  //readLevelList(levelList);
  if (readLevelsAttributs (filesList, levelList) == EXIT_FAILURE)
    {
      perror (gettext("Error when loading levels attributs from files."));
    }

  fprintf (stderr, gettext("The list of the possible levels has been generate from the content of the folder levels/.\n"));
  fprintf (stderr, "\n");

  /* count all levels from all files */
  int max_Levels = getNbrOfLevels (levelList);

  /*Load first game */

  int levelChoice = readLevelFromSetting (levelList);
  fprintf (stderr, gettext("Loading first level.\n"));
  if (loadSlcLevel (levelChoice, levelList, grid) == EXIT_SUCCESS)
    {
      fprintf (stderr, gettext("Level loaded.\n"));
    }
  else
    {
      perror (gettext("Cannot load the first level.\n"));
    }


  /* Set player position */
  int xPlayer = 0;
  int yPlayer = 0;

  /* display the level using the grid */
  if (displayLevel (grid, screen, tableSurface) == EXIT_FAILURE)
    {
      fprintf (stderr, gettext("first displayLevel failed.\n"));
      exit(EXIT_FAILURE);
    }

  /* display the top bar  */
  if (displayTopBar (levelChoice, screen, tableSurface, levelList, grid) == EXIT_FAILURE)
    {
      fprintf (stderr, gettext("first displayTopBar failed.\n"));
      exit(EXIT_FAILURE);
    }


  /* refresh the window */
  SDL_UpdateWindowSurface (window);

  /* get framerate from settings */
  int fps = getPrefAsInt ("framerate");
  if (fps < 12)
    {
      fps = 12;
    }
  fprintf (stderr, "Framerate %d\n", fps);

/* wait for quit event */


  Uint32 currentTime = 0;
  Uint32 previousTime = 0;
  int carryOn = 1, refresh = 1, x = 0, y = 0, menuOpened = 0, menuChoice = 0;
  bool freezeCommand = false;
  SDL_Event event;
  while (carryOn)
    {
      //SDL_WaitEvent(&event);
      while (SDL_PollEvent (&event))
	{

	  switch (event.type)
	    {
	    case SDL_QUIT:
	      carryOn = 0;
	      break;
	    case SDL_KEYDOWN:
	      /* Get the player position */
	      for (x = 0; x < getX_Blocks (); x++)
		{
		  for (y = 0; y < getY_Blocks (); y++)
		    {
		      if (grid[x][y].mainRole == PLAYER)
			{
			  xPlayer = x;
			  yPlayer = y;
			  //fprintf(stderr, "player on %d:%d\n", xPlayer, yPlayer);
			}
		    }
		}
	      /* listen keyboard */
	      switch (event.key.keysym.sym)
		{
		case SDLK_RIGHT:
		  /*Do not move when level is finished */
		  if (freezeCommand == true)
		    break;
		  /* Don't go outside */
		  if (xPlayer + 1 >= getX_Blocks ())
		    break;
		  /* Test if wall */
		  if (grid[xPlayer + 1][yPlayer].mainRole == WALL)
		    break;
		  /* Don't go outside with a box */
		  if (grid[xPlayer + 1][yPlayer].mainRole == BOX
		      && xPlayer + 2 >= getX_Blocks ())
		    break;
		  /* Do not move a box if it is close to a wall or an other box */
		  if (grid[xPlayer + 1][yPlayer].mainRole == BOX
		      && grid[xPlayer + 2][yPlayer].mainRole == BOX
		      && grid[xPlayer + 2][yPlayer].mainRole == WALL)
		    break;
		  /* Move a box only if there is space to do it */
		  if ((grid[xPlayer + 1][yPlayer].mainRole == BOX
		       && grid[xPlayer + 2][yPlayer].mainRole == GROUND))
		    {

		      /* clean old position */
		      grid[xPlayer][yPlayer].mainRole = GROUND;
		      grid[xPlayer][yPlayer].playerRole = EMPTY;

		      /* update new position */
		      grid[xPlayer + 2][yPlayer].mainRole = BOX;
		      grid[xPlayer + 1][yPlayer].mainRole = PLAYER;
		      grid[xPlayer + 1][yPlayer].playerRole = PLAYER_R;

		      refresh = 1;
		      break;
		    }
		  /* move only on grounds and Goals */
		  if (grid[xPlayer + 1][yPlayer].mainRole == GROUND)
		    {

		      /* clean old position */
		      grid[xPlayer][yPlayer].mainRole = GROUND;
		      grid[xPlayer][yPlayer].playerRole = EMPTY;

		      /* update new position */
		      grid[xPlayer + 1][yPlayer].mainRole = PLAYER;
		      grid[xPlayer + 1][yPlayer].playerRole = PLAYER_R;

		      refresh = 1;
		      break;
		    }
		  break;

		case SDLK_LEFT:
		  /*Do not move when level is finished */
		  if (freezeCommand == true)
		    break;

		  /* Don't go outside */
		  if (xPlayer - 1 < 0)
		    break;
		  /* test if wall */
		  if (grid[xPlayer - 1][yPlayer].mainRole == WALL)
		    break;
		  /* Don't go outside with a box */
		  if (grid[xPlayer - 1][yPlayer].mainRole == BOX
		      && xPlayer - 2 < 0)
		    break;
		  /* Do not move a box if it is close to a wall or an other box */
		  if (grid[xPlayer - 1][yPlayer].mainRole == BOX
		      && grid[xPlayer - 2][yPlayer].mainRole == BOX
		      && grid[xPlayer - 2][yPlayer].mainRole == WALL)
		    break;
		  /* Move a box only if there is space to do it */
		  if ((grid[xPlayer - 1][yPlayer].mainRole == BOX
		       && grid[xPlayer - 2][yPlayer].mainRole == GROUND))
		    {

		      /* clean old position */
		      grid[xPlayer][yPlayer].mainRole = GROUND;
		      grid[xPlayer][yPlayer].playerRole = EMPTY;

		      /* update new position */
		      grid[xPlayer - 2][yPlayer].mainRole = BOX;
		      grid[xPlayer - 1][yPlayer].mainRole = PLAYER;
		      grid[xPlayer - 1][yPlayer].playerRole = PLAYER_L;

		      refresh = 1;
		      break;
		    }
		  /* move only on grounds and Goals */
		  if (grid[xPlayer - 1][yPlayer].mainRole == GROUND)
		    {

		      /* clean old position */
		      grid[xPlayer][yPlayer].mainRole = GROUND;
		      grid[xPlayer][yPlayer].playerRole = EMPTY;

		      /* update new position */
		      grid[xPlayer - 1][yPlayer].mainRole = PLAYER;
		      grid[xPlayer - 1][yPlayer].playerRole = PLAYER_L;

		      refresh = 1;
		      break;
		    }
		  break;

		case SDLK_UP:
		  if (menuOpened == 0)
		    {
		      /*Do not move when level is finished */
		      if (freezeCommand == true)
			break;

		      /* Don't go outside */
		      if (yPlayer - 1 < 0)
			break;
		      /* test if wall */
		      if (grid[xPlayer][yPlayer - 1].mainRole == WALL)
			break;
		      /* Don't go outside with a box */
		      if (grid[xPlayer][yPlayer - 1].mainRole == BOX
			  && yPlayer - 2 < 0)
			break;
		      /* Do not move a box if it is close to a wall or an other box */
		      if (grid[xPlayer][yPlayer - 1].mainRole == BOX
			  && grid[xPlayer][yPlayer - 2].mainRole == BOX
			  && grid[xPlayer][yPlayer - 2].mainRole == WALL)
			break;
		      /* Move a box only if there is space to do it */
		      if ((grid[xPlayer][yPlayer - 1].mainRole == BOX
			   && grid[xPlayer][yPlayer - 2].mainRole == GROUND))
			{

			  /* clean old position */
			  grid[xPlayer][yPlayer].mainRole = GROUND;
			  grid[xPlayer][yPlayer].playerRole = EMPTY;

			  /* update new position */
			  grid[xPlayer][yPlayer - 2].mainRole = BOX;
			  grid[xPlayer][yPlayer - 1].mainRole = PLAYER;
			  grid[xPlayer][yPlayer - 1].playerRole = PLAYER_B;

			  refresh = 1;
			  break;
			}
		      /* move only on grounds and Goals */
		      if (grid[xPlayer][yPlayer - 1].mainRole == GROUND)
			{

			  /* clean old position */
			  grid[xPlayer][yPlayer].mainRole = GROUND;
			  grid[xPlayer][yPlayer].playerRole = EMPTY;

			  /* update new position */
			  grid[xPlayer][yPlayer - 1].mainRole = PLAYER;
			  grid[xPlayer][yPlayer - 1].playerRole = PLAYER_B;

			  refresh = 1;
			  break;
			}
		    }
		  else if (menuOpened == 1)
		    {
		      menuChoice--;
		      /* make circle */
		      if (menuChoice == -1)
			{
			  menuChoice = 2;
			}
		      openMenu (screen, tableSurface, levelList, menuChoice,
				levelChoice);
		      refresh = 1;
		    }
		  break;

		case SDLK_DOWN:
		  if (menuOpened == 0)
		    {
		      /*Do not move when level is finished */
		      if (freezeCommand == true)
			break;

		      /* Don't go outside */
		      if (yPlayer + 1 >= getY_Blocks ())
			break;
		      /* test if wall */
		      if (grid[xPlayer][yPlayer + 1].mainRole == WALL)
			break;
		      /* Don't go outside with a case */
		      if (grid[xPlayer][yPlayer + 1].mainRole == BOX
			  && yPlayer + 2 >= getY_Blocks ())
			break;
		      /* Do not move a box if it is close to a wall or an other box */
		      if (grid[xPlayer][yPlayer + 1].mainRole == BOX
			  && grid[xPlayer][yPlayer + 2].mainRole == BOX
			  && grid[xPlayer][yPlayer + 2].mainRole == WALL)
			break;
		      /* Move a box only if there is space to do it */
		      if ((grid[xPlayer][yPlayer + 1].mainRole == BOX
			   && grid[xPlayer][yPlayer + 2].mainRole == GROUND))
			{

			  /* clean old position */
			  grid[xPlayer][yPlayer].mainRole = GROUND;
			  grid[xPlayer][yPlayer].playerRole = EMPTY;

			  /* update new position */
			  grid[xPlayer][yPlayer + 2].mainRole = BOX;
			  grid[xPlayer][yPlayer + 1].mainRole = PLAYER;
			  grid[xPlayer][yPlayer + 1].playerRole = PLAYER_F;

			  refresh = 1;
			  break;
			}
		      /* move only on grounds and Goals */
		      if (grid[xPlayer][yPlayer + 1].mainRole == GROUND)
			{

			  /* clean old position */
			  grid[xPlayer][yPlayer].mainRole = GROUND;
			  grid[xPlayer][yPlayer].playerRole = EMPTY;

			  /* update new position */
			  grid[xPlayer][yPlayer + 1].mainRole = PLAYER;
			  grid[xPlayer][yPlayer + 1].playerRole = PLAYER_F;

			  refresh = 1;
			  break;
			}
		    }
		  else if (menuOpened == 1)
		    {
		      menuChoice++;
		      /* make circle */
		      if (menuChoice == 3)
			{
			  menuChoice = 0;
			}
		      openMenu (screen, tableSurface, levelList, menuChoice,
				levelChoice);
		      refresh = 1;
		    }
		  break;

		  /* hit m to open or close the menu */

		case SDLK_m:
		  if (menuOpened == 0)
		    {
		      //fprintf(stderr, "opening Menu\n");
		      freezeCommand = true;
		      menuOpened = 1;
		      refresh = 1;
		    }
		  else
		    {
		      //fprintf(stderr, "closing Menu\n");
		      /*reset status */
		      freezeCommand = false;
		      menuOpened = 0;
		      menuChoice = 0;
		      refresh = 1;
		    }
		  break;


		  /* hit r to reset the current level */
		case SDLK_r:
		  /* load the level */
		  if (loadSlcLevel (levelChoice, levelList, grid) ==
		      EXIT_SUCCESS)
		    {
		      fprintf (stderr, "Level loaded\n");
		    }
		  else
		    {
		      perror ("Impossible to load the level. Perror");
		    }


		  /*reset status */
		  freezeCommand = false;
		  menuOpened = 0;
		  refresh = 1;
		  fprintf (stderr, "Level %d\n loaded", (levelChoice + 1));
		  break;

		  /* hit n to load the next level */
		case SDLK_n:
		  /* load next the level */
		  levelChoice += 1;
		  if (levelChoice == max_Levels)
		    levelChoice = 0;
		  if (loadSlcLevel (levelChoice, levelList, grid) ==
		      EXIT_SUCCESS)
		    {
		      fprintf (stderr, "Level loaded\n");
		    }
		  else
		    {
		      perror ("Impossible to load the level. Perror");
		    }

		  /*reset status */
		  freezeCommand = false;
		  menuOpened = 0;
		  refresh = 1;
		  fprintf (stderr, "Level %d\n loaded", (levelChoice));
		  break;

		  /* hit p to load the previous level */
		case SDLK_p:
		  /* load previous level */
		  levelChoice -= 1;
		  if (levelChoice == -1)
		    levelChoice = max_Levels - 1;

		  if (loadSlcLevel (levelChoice, levelList, grid) ==
		      EXIT_SUCCESS)
		    {
		      fprintf (stderr, "Level loaded\n");
		    }
		  else
		    {
		      perror ("Impossible to load the level. Perror");
		    }

		  /*reset status */
		  freezeCommand = false;
		  menuOpened = 0;
		  refresh = 1;
		  fprintf (stderr, "Level %d\n loaded", (levelChoice));

		  break;
		  /* hit q to quit */
		case SDLK_q:
		  /* write last level name before closing */
		  strcpy (levelName, "");
		  getLevelName (levelChoice, levelList, levelName);
		  writePrefChar ("LevelName", levelName);
		  carryOn = 0;
		  fprintf (stderr,
			   "\nThe quit command (q) has been pressed.\n");
		  fprintf (stderr, "Start ending programm:\n");
		  break;

		}		//end of  switch (event.type)

	    }			//end of switch (event.type) {

	}			// end of while(SDL_PollEvent(&event))


      if (refresh == 1)
	{
       /* display the level using the grid */
	   if (displayLevel (grid, screen, tableSurface) == EXIT_FAILURE)
          {
            fprintf (stderr, gettext("displayLevel failed.\n"));
          }
	  /*display congrats if the level is finished */
	  if (levelFinished (grid) == FINISH && menuOpened == 0)
	    {
	      displayCongrats (screen, tableSurface);
	      freezeCommand = true;
	    }
	  if (menuOpened == 1)
	    {
	      openMenu (screen, tableSurface, levelList, menuChoice,
			levelChoice);
	    }

	  displayTopBar ((levelChoice), screen, tableSurface, levelList,
			 grid);

	  SDL_UpdateWindowSurface (window);
	  refresh = 0;
	}
      /* setup delay using framerate */
      currentTime = SDL_GetTicks ();
      if ((currentTime - previousTime) < (unsigned) (1000 / fps))
	{
	  SDL_Delay ((unsigned) (1000 / fps) - (currentTime - previousTime));

	}
      else
	{
	  previousTime = currentTime;
	}

    }				// end of   while (carryOn){


  /* clean */
  fprintf (stderr, "- Destroying level list\n");
  destroy (levelList);
  free (levelList);
  fprintf (stderr, "- Destroying file list\n");
  destroyFileList (filesList);
  free (filesList);
  fprintf(stderr,"-Freeing Sprites\n");
  freeSprites(tableSurface);
  SDL_FreeSurface (screen);
  fprintf (stderr, "- Free SDL screen\n");
  SDL_DestroyWindow (window);
  fprintf (stderr, "- Free SDL window\n");
  TTF_Quit ();
  fprintf (stderr, "- TFF is out\n");
  SDL_Quit ();
  fprintf (stderr, "- SDL is out\n");
  fprintf (stderr, "Everythings seems good, Bye\n");
  return EXIT_SUCCESS;
}
