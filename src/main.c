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
#include "../inc/mouse.h"
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

  /* Declare display mode structure */
  SDL_DisplayMode current;

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
  loadPrefStruct(current);


  printf (gettext ("The window size request from settings is %dx%d\n"),
	  pref.window_width, pref.window_height);

  /* Create the window game */
  SDL_Window *window = SDL_CreateWindow (GAME_NAME,
					 SDL_WINDOWPOS_UNDEFINED,
					 SDL_WINDOWPOS_UNDEFINED,
					 pref.window_width,
					 pref.window_height,
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

  S_Text tableTextSurface[NBR_OF_TAB][NBR_OF_TAB_LINE];
  tableTextSurface_init (tableTextSurface);

  /* create a grid using the heigth and width form settings */
  Square grid[pref.max_X_Blocks][pref.max_Y_Blocks];
  grid_init (grid);

  /* create a sub grid for dispatch items on the menu */
  S_Menu gridMenu[pref.menu_X_Blocks][pref.menu_Y_Blocks];
  gridMenu_init (gridMenu);

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


  /*init Menu Choice*/
  menuChoice.tabChoice = 1;
  menuChoice.lineChoice = 0;
  menuChoice.nbrTabs = NBR_OF_TAB - 1;
  menuChoice.open = 0;
  menuChoice.freeze = 0;
  menuChoice.xPos = 2;
  menuChoice.yPos = 1;

  menuChoice.tab[0].role = M_EMPTY;
  menuChoice.tab[0].nbrLines = 0;
  menuChoice.tab[0].name = TAB_EMPTY ;

  menuChoice.tab[1].role = M_INFO;
  menuChoice.tab[1].nbrLines = MENU_MAX_INFO;
  menuChoice.tab[1].name = INFO;

  menuChoice.tab[2].role = M_FILE;
  menuChoice.tab[2].nbrLines = MENU_MAX_FILE;
  menuChoice.tab[2].name = FILES;

  menuChoice.tab[3].role = M_SHORTCUTS;
  menuChoice.tab[3].nbrLines = MENU_MAX_SHORTCUTS;
  menuChoice.tab[3].name = SHORTCUTS;

  menuChoice.tab[4].role = M_SETTINGS;
  menuChoice.tab[4].nbrLines = MENU_MAX_SETTINGS;
  menuChoice.tab[4].name = SETTINGS;

  menuChoice.tab[5].role = M_ABOUT;
  menuChoice.tab[5].nbrLines = MENU_MAX_ABOUT;
  menuChoice.tab[5].name = ABOUT;

  /*Load first game */
  pref.level = readLevelFromSetting (levelList);
  pref.level_max = getNbrOfLevels (levelList);

  fprintf (stderr, gettext ("Loading first level.\n"));

  if (loadSlcLevel (levelList, grid) == EXIT_SUCCESS)
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
      (screen, tableSurface, levelList,
       grid, gridMenu) == EXIT_FAILURE)
    {
      fprintf (stderr, gettext ("first displayTopBar() failed.\n"));
      exit (EXIT_FAILURE);
    }


  /* refresh the window */
  SDL_UpdateWindowSurface (window);

  /* display framerate from settings */
  fprintf (stderr, "Framerate %d\n", pref.framerate);

/* wait for quit event */

  char levelName[MAX_CARACT] = "";
  Uint32 currentTime = 0;
  Uint32 previousTime = 0;
  int carryOn = 1, refresh = 1, reload = 0, target = STILL, next_target = STILL;


  int xCursor = 0, yCursor = 0;

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
      /*get x cursor position */
        if (event.button.x < 0)
          xCursor = 0;
        else if(event.button.x > pref.window_height)
          xCursor = pref.window_height;
        else
	        xCursor = event.button.x;

      /*get y cursor position */
        if (event.button.y < 0)
          yCursor = 0;
        else if(event.button.y > pref.window_width)
          yCursor = pref.window_width;
        else
	        yCursor = event.button.y;

        if (menuChoice.open == 1){
        refresh = mouseMotion(xCursor,yCursor,gridMenu);
        }

	      /*Do not move when level is finished */
	      if (menuChoice.freeze == 1 )
		break;

	      if (menuChoice.open == 0)
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

		  if (menuChoice.open == 1)
		    {
		      /*Clic outside the open menu to close it */
		      if ((xCursor < pref.x_menu
			   || xCursor > pref.x_menu + MENU_WIDTH)
			  || (yCursor < SPRITE_SIZE
			      || yCursor > 7 * SPRITE_SIZE))
			{
              refresh = openCloseTheMenu();
			  break;
			}
		    }

		  if (menuChoice.open == 0)
		    {
		      /*Clic on the top bar top open the menu */
		      if (xCursor > pref.x_menu
			  && xCursor < pref.x_menu + MENU_WIDTH)
			{
			  if (yCursor < SPRITE_SIZE)
			    {
                refresh = openCloseTheMenu();
			    break;
			    }
			}

		      getPosPlayer (&xPlayer, &yPlayer, grid);
		      /*Do not move when level is finished */
		      if (menuChoice.freeze == 1)
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
        if (menuChoice.open == 0)
		    {
		      /*Do not move when level is finished */
		      if (menuChoice.freeze == 1)
			        break;

		      refresh = movePlayer (xPlayer, yPlayer, RIGHT, grid);
		      break;
		    }

		  else
		    {
          menuChoice.xPos++;
                     if (menuChoice.xPos > 6)
                         menuChoice.xPos = 2;

          menuChoice.tabChoice++;
          if (menuChoice.tabChoice > menuChoice.nbrTabs)
              menuChoice.tabChoice = 1;

		      refresh = 1;
		    }
		  break;

		case SDLK_LEFT:
		  if (menuChoice.open == 0)
		    {
		      /*Do not move when level is finished */
		      if (menuChoice.freeze == 1)
			break;

		      refresh = movePlayer (xPlayer, yPlayer, LEFT, grid);
		      break;
		    }
		  else
		    {
          menuChoice.xPos--;
          if (menuChoice.xPos < 2)
              menuChoice.xPos = 6;

          menuChoice.tabChoice--;
          if (menuChoice.tabChoice < 1)
              menuChoice.tabChoice = menuChoice.nbrTabs;
		      refresh = 1;
		    }
		  break;

		case SDLK_UP:
		  if (menuChoice.open == 0)
		    {
		      /*Do not move when level is finished */
		      if (menuChoice.freeze == 1)
			break;

		      refresh = movePlayer (xPlayer, yPlayer, UP, grid);
		      break;
		    }
		  else
		    {
		      menuChoice.lineChoice--;
		      /* make circle */
		      if (menuChoice.lineChoice < 0)
			{
			  menuChoice.lineChoice = (menuChoice.tab[menuChoice.tabChoice].nbrLines);
			}
		      refresh = 1;
		    }
		  break;

		case SDLK_DOWN:
		  if (menuChoice.open == 0)
		    {
		      /*Do not move when level is finished */
		      if (menuChoice.freeze == 1)
			break;

		      refresh = movePlayer (xPlayer, yPlayer, DOWN, grid);
		      break;
		    }

		  else
		    {
		      menuChoice.lineChoice++;
		      /* make circle */
		      if (menuChoice.lineChoice > menuChoice.tab[menuChoice.tabChoice].nbrLines)
			{
			  menuChoice.lineChoice = 0;
			}
		      refresh = 1;
		    }
		  break;

		  /* hit m to open or close the menu */

		case SDLK_m:
            refresh = openCloseTheMenu();
		  break;


		  /* hit r to reset the current level */
		case SDLK_r:
		  /* load the level */
		  reload = 1;
		  refresh = 1;
		  break;

		  /* hit n to load the next level */
		case SDLK_n:
		  /* load next the level */
		  pref.level += 1;
		  reload = 1;
		  refresh = 1;
		  break;

		  /* hit p to load the previous level */
		case SDLK_p:
		  /* load previous level */
		  pref.level -= 1;

      reload = 1;
		  refresh = 1;
		  break;

		  /* hit q to quit */
		case SDLK_q:
		  /* write last level name before closing */
		  strcpy (levelName, "");
		  getLevelName (pref.level, levelList, levelName);
		  writePrefChar ("LevelName", levelName);
		  carryOn = 0;
		  fprintf (stderr,
			   "\nThe quit command (q) has been pressed.\n");
		  fprintf (stderr, "Start ending programm:\n");
		  break;

		}		//end of  switch (event.type)

	    }			//end of switch (event.type)

	}			// end of while(SDL_PollEvent(&event))

    //reload the level from file if necessary
    if(reload == 1){
      //loop if level < 0 or > level_max
      if (pref.level == -1)
		    pref.level = pref.level_max - 1;
		  else if (pref.level == pref.level_max)
		    pref.level = 0;

      if (loadSlcLevel (levelList, grid) ==
		      EXIT_FAILURE)
		      perror ("Impossible to load the level. Perror");
      reload = 0;
    }
      if (refresh == 1)
	{
	  /* display the level using the grid */
	  if (displayLevel (grid, screen, tableSurface) == EXIT_FAILURE)
	    {
	      fprintf (stderr, gettext ("displayLevel() failed.\n"));
	    }
	  /*display congrats if the level is finished */
	  if (levelFinished (grid) == FINISH && menuChoice.open == 0)
	    {
	      displayCongrats (screen, tableSurface);
	      menuChoice.freeze = 1;
	    }
	  if (menuChoice.open == 1)
	    {
	      openMenu (screen, tableSurface, tableTextSurface, levelList, gridMenu);
	    }

	  displayTopBar (screen, tableSurface,
			 levelList, grid, gridMenu);

	  SDL_UpdateWindowSurface (window);
	  refresh = 0;
	}
      /* setup delay using framerate */
      currentTime = SDL_GetTicks ();
      if ((currentTime - previousTime) < (unsigned) (1000 / pref.framerate))
	{
	  SDL_Delay ((unsigned) (1000 / pref.framerate) - (currentTime - previousTime));

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
