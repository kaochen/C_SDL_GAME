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

#include "const.h"
#include "level.h"
#include "level_decor.h"
#include "menu.h"
#include "menu_text.h"
#include "mouse.h"
#include "move.h"
#include "settings.h"
#include "sprites.h"
#include "slc.h"


int
main (int argc, char *argv[])
{
  /* Set up gettext for internationalisation */
  setlocale (LC_ALL, "");
  textdomain ("sokorobot");
  bindtextdomain ("sokorobot", LOCALEDIR);

  /*Manage Program Arguments */
  int i;
  bool vb = false; //verbose mode

  for (i = 1; i < argc; i++){
      printf (gettext ("Options argv[%d]:%s \t"), i, argv[i]);
      int test = strcmp(argv[i], "-v");
      if (test == 0 ){
          vb = true;
          fprintf(stderr, gettext("Verbose mode is on\n"));
      }
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
      fprintf (stderr, "SDL_Init() failed: %s\n",
	       SDL_GetError ());
      exit (EXIT_FAILURE);
    }

  /* Start SDL TTF */
  if (TTF_Init () == -1)
    {
      fprintf (stderr, "TTF_Init() failed: %s\n",
	       TTF_GetError ());
      exit (EXIT_FAILURE);
    }

  /*load preferences */
  vbPrintf(gettext("Current directory %s\n"), argv[0]);
  setupPath(argv[0]);
  loadPrefStruct(current);
  pref.verbosity = vb;


  vbPrintf (gettext ("The window size request from settings is %dx%d\n"),
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
      fprintf (stderr, "SDL_CreateWindow() failed: %s\n",
	       SDL_GetError ());
      exit (EXIT_FAILURE);
    }

  /* Set window icon */
  char iconPath[MAX_PATH];
  sprintf (iconPath, "%sicon.png",pref.imgPath);
  vbPrintf (gettext ("Game icon path: %s\n"),iconPath);
  SDL_SetWindowIcon (window, IMG_Load (iconPath));

  /* Create a surface from the window */
  SDL_Surface *screen = NULL;
  screen = SDL_GetWindowSurface (window);

  /* Check if window and surface are created */
  if (screen == NULL)
    {
      fprintf (stderr, "SDL_GetWindowSurface() failed: %s\n",
	       SDL_GetError ());
      exit (EXIT_FAILURE);
    }

  vbPrintf (gettext ("The SDL window has been created.\n\n"));

  /* load images into a table of struct */
  Sprites tableSurface[NBR_OF_IMAGES];

  for (i = 0; i < NBR_OF_IMAGES; i++)
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
  S_Menu gridMenu[pref.max_X_Blocks][pref.max_Y_Blocks];
  gridMenu_init (gridMenu);

  /*List slc files from the LEVELS_FOLDER */
  S_FilesList *filesList = initFilesList ();
  if (listSlcLevelFiles (filesList) == EXIT_FAILURE)
    {
      vbPrintf (gettext
	      ("The level files cannot be listed from the folder %s."), pref.levelsPath);
    }

  /*Read files name from the filesList to check */
  if (readFilesList (filesList) == EXIT_FAILURE)
    {
      perror ("readFilesList() failed.");
    }
  loadFileName(tableTextSurface,filesList);

  /*Read level from slc file */
  S_LevelList *levelList =initLevelList ();
  //readLevelList(levelList);
  if (readLevelsAttributs (filesList, levelList) == EXIT_FAILURE)
    {
      perror (gettext ("Error when loading levels attributs from files."));
    }

  vbPrintf (gettext
	   ("The list of the possible levels has been generate from the content of the folder "LEVELS_FOLDER".\n"));
  fprintf (stderr, "\n");

  /* init menu choice,status*/
  menuChoiceInit();

  /*Load first game */
  pref.level = readLevelFromSetting (levelList);
  pref.level_max = getNbrOfLevels (levelList);

  fprintf (stderr, gettext ("Loading first level.\n"));

  if (loadSlcLevel (levelList, grid, gridMenu) == EXIT_FAILURE)
    {
      perror (gettext ("Cannot load the first level.\n"));
    }


  /* Set player position */
  int xPlayer = 0;
  int yPlayer = 0;

  /* display the level using the grid */
  if (displayLevel (grid, screen, tableSurface) == EXIT_FAILURE)
    {
      fprintf (stderr, "first displayLevel() failed.\n");
      exit (EXIT_FAILURE);
    }

  /* display the top bar  */
  if (displayTopBar
      (screen, tableSurface, levelList,
       grid, gridMenu) == EXIT_FAILURE)
    {
      fprintf (stderr, "first displayTopBar() failed.\n");
      exit (EXIT_FAILURE);
    }


  /* refresh the window */
  SDL_UpdateWindowSurface (window);

  /* display framerate from settings */
  vbPrintf (gettext("Framerate %d\n"), pref.framerate);

/* wait for quit event */

  Uint32 currentTime = 0;
  Uint32 previousTime = 0;
  int carryOn = 1, refresh = 1,  target = STILL, next_target = STILL;


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
      int xBCursor = xCursor/SPRITE_SIZE; //to get the block coordonate.
      int yBCursor = yCursor/SPRITE_SIZE;

      /*clic on buttons of the top bar */
      if (gridMenu[xBCursor][yBCursor].role != M_EMPTY &&
             gridMenu[xBCursor][yBCursor].type == TOPBAR ){
            switch (gridMenu[xBCursor][yBCursor].role)
              {
              case M_PREVIOUS:
                  refresh = changeLevel(-1);
                break;
              case M_NEXT:
                  refresh = changeLevel(1);
                break;
              case M_RESET:
                  refresh = changeLevel(0);
                break;
              }
        break;
        }

		  /*Open and close the menu */
		  if (menuChoice.open == 1)
		    {
		      /*Clic outside the open menu to close it */
		     if ((gridMenu[xBCursor][yBCursor].role == M_EMPTY &&
             gridMenu[xBCursor][yBCursor].type == TOPBAR ) ||
             gridMenu[xBCursor][yBCursor].type == MAIN_WINDOW)
            {
                refresh = openCloseTheMenu(gridMenu);
                break;
			      }
        }

		  if (menuChoice.open == 0)
		    {
		      /*Clic on the top bar top open the menu */
		     if (gridMenu[xBCursor][yBCursor].role == M_EMPTY &&
             gridMenu[xBCursor][yBCursor].type == TOPBAR)
             {
                refresh = openCloseTheMenu(gridMenu);
                break;
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
                     if (menuChoice.xPos > (pref.xb_menu + 6))
                         menuChoice.xPos = (pref.xb_menu + 2);

          menuChoice.tabChoice++;
			    menuChoice.lineChoice = MENU_OFFSET;
          if (menuChoice.tabChoice > menuChoice.nbrTabs)
              menuChoice.tabChoice = 1;

          gridMenu_initLines(gridMenu);
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
          if (menuChoice.xPos < (pref.xb_menu + 2))
              menuChoice.xPos = (pref.xb_menu + 6);

          menuChoice.tabChoice--;
			    menuChoice.lineChoice = MENU_OFFSET;
          if (menuChoice.tabChoice < 1)
              menuChoice.tabChoice = menuChoice.nbrTabs;

          gridMenu_initLines(gridMenu);
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
		      if (menuChoice.lineChoice < MENU_OFFSET)
			{
			  menuChoice.lineChoice = (menuChoice.tab[menuChoice.tabChoice].nbrLines +2);
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
		      if (menuChoice.lineChoice > menuChoice.tab[menuChoice.tabChoice].nbrLines +2)
			{
			  menuChoice.lineChoice = MENU_OFFSET;
			}
		      refresh = 1;
		    }
		  break;

		  /* hit m to open or close the menu */

		case SDLK_m:
            refresh = openCloseTheMenu(gridMenu);
            gridMenu_initLines(gridMenu);
		  break;


		  /* hit r to reset the current level */
		case SDLK_r:
		  /* load the level */
      refresh = changeLevel(0);
		  break;

		  /* hit n to load the next level */
		case SDLK_n:
		  /* load next the level */
      refresh = changeLevel(1);
		  break;

		  /* hit p to load the previous level */
		case SDLK_p:
		  /* load previous level */
      refresh = changeLevel(-1);
		  break;

		  /* hit q to quit */
		case SDLK_q:
		  /* write last level name before closing */

		  carryOn = 0;
		  fprintf (stderr,gettext(
			   "\nThe quit command (q) has been pressed.\n"));
		  break;

		}		//end of  switch (event.type)

	    }			//end of switch (event.type)

	}			// end of while(SDL_PollEvent(&event))

    //reload the level from file if necessary
    if(pref.reload == 1){
      //loop if level < 0 or > level_max
      if (pref.level == -1)
		    pref.level = pref.level_max - 1;
		  else if (pref.level == pref.level_max)
		    pref.level = 0;

      if (loadSlcLevel (levelList, grid, gridMenu) ==
		      EXIT_FAILURE)
		      perror ("loadSlcLevel() failed. Perror");
      pref.reload = 0;
    }
      if (refresh == 1)
	{
	  /* display the level using the grid */
	  if (displayLevel (grid, screen, tableSurface) == EXIT_FAILURE)
	    {
	      fprintf (stderr, "displayLevel() failed.\n");
	    }
	  /*display congrats if the level is finished */
	  if (levelFinished (grid) == FINISH && menuChoice.open == 0)
	    {
	      displayCongrats (screen, tableSurface);
        gridMenu[pref.xb_menu + 9][0] = (S_Menu) {.role=M_CHECKED, .type=TOPBAR, .tab=0, .image=BUTTON_CHECKED};
        S_Level *current = malloc (sizeof (S_Level));
        getCurrentLevelInfos(levelList, current);
        storeLevelsFinished(pref.sessionPath, current->name);
        free(current);
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
  //store the current level name before closing
	S_Level *currentLevel = malloc (sizeof (S_Level));
  getCurrentLevelInfos(levelList, currentLevel);
	writeChar (pref.sessionPath, "LevelName", currentLevel->name);
  free (currentLevel);

  /* clean */
  destroy (levelList);
  free (levelList);
  destroyFileList (filesList);
  free (filesList);
  freeS_Text (tableTextSurface);
  freeSprites (tableSurface);
  SDL_FreeSurface (screen);
  SDL_DestroyWindow (window);
  TTF_Quit ();
  SDL_Quit ();
  fprintf (stderr, gettext("Bye\n"));
  return EXIT_SUCCESS;
}
