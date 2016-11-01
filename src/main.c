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
#include "../inc/level.h"
#include "../inc/level_decor.h"
#include "../inc/menu.h"
#include "../inc/menu_text.h"
#include "../inc/move.h"
#include "../inc/settings.h"
#include "../inc/sprites.h"
#include "../inc/slc.h"


int
main (int argc, char *argv[])
{
  /* Set up gettext for internationalisation */
  setlocale (LC_ALL, "");
  textdomain ("sokorobot");
  bindtextdomain ("sokorobot", "/usr/share/locale");

  /*manage argument (not used for now) */
  int i;

  for (i = 1; i < argc; i++)
    {
      printf (gettext ("Argument %d : %s \n"), i, argv[i]);
    }

  /* reset errors */
  errno = 0;

  /* start random processor just once */
  srand (time (NULL));

  /* Start and check if SDL start correctly */
  if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_TIMER) == -1)
    {
      fprintf (stderr, gettext ("SDL initialization error: %s\n"),
	       SDL_GetError ());
      exit (EXIT_FAILURE);
    }

  /* Start SDL TTF */
  if (TTF_Init () == -1)
    {
      fprintf (stderr, gettext ("TTF_Init initialization error: %s\n"),
	       TTF_GetError ());
      exit (EXIT_FAILURE);
    }

  /*load preferences */

  printf (gettext ("The window size request from settings is %dx%d\n"),
	  getWindow_width (), getWindow_height ());

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
      fprintf (stderr, gettext ("SDL_CreateWindow failed: %s\n"),
	       SDL_GetError ());
      exit (EXIT_FAILURE);
    }

  /* Set window icon */
  SDL_SetWindowIcon (window, IMG_Load ("img/original/icon.png"));

  /* Create a surface from the window */
  SDL_Surface *screen = NULL;
  screen = SDL_GetWindowSurface (window);

  /* Check if window and surface are created */
  if (screen == NULL)
    {
      fprintf (stderr, gettext ("SDL_GetWindowSurface failed: %s\n"),
	       SDL_GetError ());
      exit (EXIT_FAILURE);
    }

  fprintf (stderr, gettext ("The SDL window has been created.\n"));

  fprintf (stderr, "\n");
  /* load images into a table of struct */
  Sprites tableSurface[NBR_OF_IMAGES];

  for (i = 0; i <= NBR_OF_IMAGES; i++)
    {
      tableSurface[i].image = 0;
    }
  loadAllSprites (tableSurface);

  /* load text into a table of struct */

  S_Text tableTextSurface[NBR_OF_TEXT];
  tableTextSurface_init (tableTextSurface);

  /* create a grid using the heigth and width form settings */

  int h = getMax_Y_Blocks (), w = getMax_X_Blocks ();
  Square grid[w][h];
  grid_init (w, h, grid);




  fprintf (stderr, "\n");
  /*List slc files from the levels/ folder */
  S_FilesList *filesList = initFilesList ();
  if (listSlcLevelFiles (filesList) == EXIT_FAILURE)
    {
      perror (gettext
	      ("The level files cannot cannot be listed from the folder levels/."));
    }

  fprintf (stderr, "\n");
  /*Read files name from the filesList to check */
  if (readFilesList (filesList) == EXIT_FAILURE)
    {
      perror (gettext ("The level list cannot be verified."));
    }


  fprintf (stderr, "\n");

  /*Read level from slc file */
  S_LevelList *levelList = initLevelList ();
  //readLevelList(levelList);
  if (readLevelsAttributs (filesList, levelList) == EXIT_FAILURE)
    {
      perror (gettext ("Error when loading levels attributs from files."));
    }

  fprintf (stderr,
	   gettext
	   ("The list of the possible levels has been generate from the content of the folder levels/.\n"));
  fprintf (stderr, "\n");

  /* count all levels from all files */
  int max_Levels = getNbrOfLevels (levelList);

  /*Load first game */

  int levelChoice = readLevelFromSetting (levelList);
  fprintf (stderr, gettext ("Loading first level.\n"));
  if (loadSlcLevel (levelChoice, levelList, grid) == EXIT_SUCCESS)
    {
      fprintf (stderr, gettext ("Level loaded.\n"));
    }
  else
    {
      perror (gettext ("Cannot load the first level.\n"));
    }


  /* Set player position */
  int xPlayer = 0;
  int yPlayer = 0;

  /* display the level using the grid */
  if (displayLevel (grid, screen, tableSurface) == EXIT_FAILURE)
    {
      fprintf (stderr, gettext ("first displayLevel() failed.\n"));
      exit (EXIT_FAILURE);
    }

  /* display the top bar  */
  if (displayTopBar
      (levelChoice, screen, tableSurface, levelList,
       grid) == EXIT_FAILURE)
    {
      fprintf (stderr, gettext ("first displayTopBar() failed.\n"));
      exit (EXIT_FAILURE);
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

  char levelName[MAX_CARACT] = "";
  Uint32 currentTime = 0;
  Uint32 previousTime = 0;
  int carryOn = 1, refresh = 1, menuOpened = 0, menuChoice = 0, target =
    STILL, next_target = STILL;
  int xCursor = 0, yCursor = 0;
  bool freezeCommand = false;

  SDL_Event event;
  Uint32 windowID = SDL_GetWindowID (window);

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

	    case SDL_WINDOWEVENT:
	      {
		if (event.window.windowID == windowID)
		  {
		    switch (event.window.event)
		      {

		      case SDL_WINDOWEVENT_EXPOSED:
			{
			  refresh = 1;
			  break;
			}

		      }
		  }
	      }
	    case SDL_MOUSEMOTION:

	      xCursor = event.button.x;
	      yCursor = event.button.y;
	      if (menuOpened == 1)
		{
		  if (xCursor > menuPosX ()
		      && xCursor < menuPosX () + MENU_WIDTH)
		    {
		      if (yCursor < 90)
			menuChoice = 0;
		      if (yCursor >= 90 && yCursor < 3 * SPRITE_SIZE)
			menuChoice = 1;
		      if (yCursor >= 3 * SPRITE_SIZE)
			menuChoice = 2;
		    }
		  refresh = 1;
		  break;
		}

	      /*Do not move when level is finished */
	      if (freezeCommand == true)
		break;

	      if (menuOpened == 0)
		{
		  /*Place the target image between the cursor and the player to indicate the next move */
		  getPosPlayer (&xPlayer, &yPlayer, grid);
		  next_target =
		    mouseMoveDirection (xPlayer, yPlayer, xCursor, yCursor);

		  if (target != next_target)
		    {
		      moveTarget (next_target, xPlayer, yPlayer, grid);
		      refresh = 1;
		      target = next_target;
		      break;
		    }
		}
	      break;

	    case SDL_MOUSEBUTTONUP:
	      if (event.button.button == SDL_BUTTON_LEFT)
		{
		  xCursor = event.button.x;
		  yCursor = event.button.y;
		  /*Open and close the menu */

		  if (menuOpened == 1)
		    {
		      /*Clic outside the open menu to close it */
		      if ((xCursor < menuPosX ()
			   || xCursor > menuPosX () + MENU_WIDTH)
			  || (yCursor < SPRITE_SIZE
			      || yCursor > 7 * SPRITE_SIZE))
			{
			  menuOpened = 0;
			  refresh = 1;
			  break;
			}
		    }

		  if (menuOpened == 0)
		    {
		      /*Clic on the top bar top open the menu */
		      if (xCursor > menuPosX ()
			  && xCursor < menuPosX () + MENU_WIDTH)
			{
			  if (yCursor < SPRITE_SIZE)
			    {
			      menuOpened = 1;
			      refresh = 1;
			      break;
			    }
			}

		      getPosPlayer (&xPlayer, &yPlayer, grid);
		      /*Do not move when level is finished */
		      if (freezeCommand == true)
			break;

		      int direction =
			mouseMoveDirection (xPlayer, yPlayer, xCursor,
					    yCursor);

		      refresh =
			movePlayer (xPlayer, yPlayer, direction, grid);
		      //reset target status if move
		      target = STILL;
		    }

		}
	      break;

	    case SDL_KEYDOWN:
	      /* Get the player position */
	      getPosPlayer (&xPlayer, &yPlayer, grid);


	      /* listen keyboard */
	      switch (event.key.keysym.sym)
		{
		case SDLK_RIGHT:
		  /*Do not move when level is finished */
		  if (freezeCommand == true)
		    break;

		  refresh = movePlayer (xPlayer, yPlayer, RIGHT, grid);
		  break;

		case SDLK_LEFT:
		  /*Do not move when level is finished */
		  if (freezeCommand == true)
		    break;

		  refresh = movePlayer (xPlayer, yPlayer, LEFT, grid);
		  break;

		case SDLK_UP:
		  if (menuOpened == 0)
		    {
		      /*Do not move when level is finished */
		      if (freezeCommand == true)
			break;

		      refresh = movePlayer (xPlayer, yPlayer, UP, grid);
		      break;
		    }
		  else
		    {
		      menuChoice--;
		      /* make circle */
		      if (menuChoice == -1)
			{
			  menuChoice = 2;
			}
		      openMenu (screen, tableSurface, tableTextSurface,
				levelList, menuChoice, levelChoice);
		      refresh = 1;
		    }
		  break;

		case SDLK_DOWN:
		  if (menuOpened == 0)
		    {
		      /*Do not move when level is finished */
		      if (freezeCommand == true)
			break;

		      refresh = movePlayer (xPlayer, yPlayer, DOWN, grid);
		      break;
		    }

		  else
		    {
		      menuChoice++;
		      /* make circle */
		      if (menuChoice == 3)
			{
			  menuChoice = 0;
			}
		      openMenu (screen, tableSurface, tableTextSurface,
				levelList, menuChoice, levelChoice);
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

	    }			//end of switch (event.type)

	}			// end of while(SDL_PollEvent(&event))


      if (refresh == 1)
	{
	  /* display the level using the grid */
	  if (displayLevel (grid, screen, tableSurface) == EXIT_FAILURE)
	    {
	      fprintf (stderr, gettext ("displayLevel() failed.\n"));
	    }
	  /*display congrats if the level is finished */
	  if (levelFinished (grid) == FINISH && menuOpened == 0)
	    {
	      displayCongrats (screen, tableSurface);
	      freezeCommand = true;
	    }
	  if (menuOpened == 1)
	    {
	      openMenu (screen, tableSurface, tableTextSurface, levelList,
			menuChoice, levelChoice);
	    }

	  displayTopBar ((levelChoice), screen, tableSurface,
			 levelList, grid);

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

    }				// end of   while (carryOn)


  /* clean */
  fprintf (stderr, "- Destroying level list\n");
  destroy (levelList);
  free (levelList);
  fprintf (stderr, "- Destroying file list\n");
  destroyFileList (filesList);
  free (filesList);
  fprintf (stderr, "-Freeing texts\n");
  freeS_Text (tableTextSurface);
  fprintf (stderr, "-Freeing Sprites\n");
  freeSprites (tableSurface);
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
