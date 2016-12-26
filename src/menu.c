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

/*This file handles all the function needed to build the menu.*/

#ifndef MENU_C
#define MENU_C

#include "../inc/menu.h"

#include <libintl.h>
#include <locale.h>

#include <sys/stat.h>

/* Init menu grid */
void
gridMenu_init(S_preferences *pref,
              S_Menu gridMenu[pref->menu_X_Blocks][pref->menu_Y_Blocks])
{
   for(int y=0; y < pref->menu_Y_Blocks; ++y)
      for(int x=0; x< pref->menu_X_Blocks; ++x)
      gridMenu[x][y] = (S_Menu) {.role=M_EMPTY, .type=MAIN_WINDOW, .text=TAB_EMPTY, .image=NO_IMAGE, .nbr_of_lines = 1};

    //place elements
   gridMenu[2][0] = (S_Menu) {.role=M_PREVIOUS, .type=TOPBAR, .text=TAB_EMPTY, .image=BUTTON_ARROW_L, .nbr_of_lines = 1};
   gridMenu[7][0] = (S_Menu) {.role=M_NEXT, .type=TOPBAR,  .text=TAB_EMPTY, .image=BUTTON_ARROW_P, .nbr_of_lines = 1};
   gridMenu[8][0] = (S_Menu) {.role=M_RESET, .type=TOPBAR, .text=TAB_EMPTY, .image=BUTTON_RESET, .nbr_of_lines = 1};
   gridMenu[9][0] = (S_Menu) {.role=M_BACKWARDS, .type=TOPBAR, .text=TAB_EMPTY, .image=BUTTON_BACKWARDS, .nbr_of_lines = 1};
   gridMenu[2][1] = (S_Menu) {.role=M_INFO, .type=TABS, .text=INFO, .image=BUTTON_LEVELS, .nbr_of_lines = 5};
   gridMenu[3][1] = (S_Menu) {.role=M_FILE, .type=TABS, .text=FILES,  .image=BUTTON_FILE, .nbr_of_lines = 4};
   gridMenu[4][1] = (S_Menu) {.role=M_SHORTCUTS, .type=TABS, .text=SHORTCUTS, .image=BUTTON_SHORTCUTS, .nbr_of_lines = 6};
   gridMenu[5][1] = (S_Menu) {.role=M_SETTINGS, .type=TABS, .text=SETTINGS,  .image=BUTTON_SETTINGS, .nbr_of_lines = 2};
   gridMenu[6][1] = (S_Menu) {.role=M_ABOUT, .type=TABS, .text=ABOUT, .image=BUTTON_ABOUT, .nbr_of_lines = 4};

}


/* display Top Bar*/
int
displayTopBar (S_preferences * pref,
	       SDL_Surface * screen,
	       Sprites tableSurface[NBR_OF_IMAGES],
	       S_LevelList * levelList,
	       Square grid[pref->max_X_Blocks][pref->max_Y_Blocks],
         S_Menu gridMenu[pref->menu_X_Blocks][pref->menu_Y_Blocks])
{
  /* first add background */
  if (backgroundTopBar (pref, screen, tableSurface) == EXIT_FAILURE)
    {
      fprintf (stderr, gettext ("backgroundTopBar failed.\n"));
      return EXIT_FAILURE;
    }

  /* display the level number */
  if (levelMenu (pref, screen, levelList) == EXIT_FAILURE)
    {
      fprintf (stderr, gettext ("levelMenu failed.\n"));
      return EXIT_FAILURE;
    }

  /* display the buttons */
  if (displayTopBarButtons (pref, screen, tableSurface, gridMenu) == EXIT_FAILURE)
    {
      fprintf (stderr, gettext ("displayTopBarButtons() failed.\n"));
      return EXIT_FAILURE;
    }

  /* display the circle */
  if (displayProgress (pref, grid, screen, tableSurface) == EXIT_FAILURE)
    {
      fprintf (stderr, gettext ("displayProgress failed.\n"));
      return EXIT_FAILURE;
    }

  /* display the player into the circle */
  SDL_Rect facePos;
  facePos.x = pref->x_menu + 24;
  facePos.y = 12;
  SDL_BlitSurface (tableSurface[PLAYER_FRONT].image, NULL, screen, &facePos);
  return EXIT_SUCCESS;

}

/* display background of the top bar menu */
int
backgroundTopBar (S_preferences * pref,
                  SDL_Surface * screen,
                  Sprites tableSurface[NBR_OF_IMAGES])
{
  if (screen == NULL || tableSurface == NULL)
    {
      fprintf (stderr, gettext ("init backgroundTopBar failed: %s\n"),
	       SDL_GetError ());
      return EXIT_FAILURE;
    }

  SDL_Rect menuPos;
  menuPos.x = pref->x_menu;
  menuPos.y = 0;
  SDL_BlitSurface (tableSurface[MENU_BAR].image, NULL, screen, &menuPos);
  return EXIT_SUCCESS;

}


/* count how many goals are need to complete the level */
int
goalLeft (S_preferences *pref,
              Square grid[pref->max_X_Blocks][pref->max_Y_Blocks])
{
  int x = 0, y = 0, nbrOfBoxOk = 0;
  for (x = 0; x < pref->max_X_Blocks; x++)
    {
      for (y = 0; y < pref->max_Y_Blocks; y++)
	{
	  if (grid[x][y].mainRole == BOX && grid[x][y].goalRole == GOAL)
	    nbrOfBoxOk += 1;
	}
    }
  return nbrOfBoxOk;
}

/* count goals all ready achieve */
int
nbr_of_goals (S_preferences *pref,
              Square grid[pref->max_X_Blocks][pref->max_Y_Blocks])
{
  int x = 0, y = 0, nbrOfGoal = 0;
  for (x = 0; x < pref->x_Blocks; x++)
    {
      for (y = 0; y < pref->y_Blocks; y++)
	{
	  if (grid[x][y].goalRole == GOAL)
	    nbrOfGoal += 1;
	}
    }
  return nbrOfGoal;
}

/* Display Progress in the level */
int
displayProgress (S_preferences * pref,
       Square grid[pref->max_X_Blocks][pref->max_Y_Blocks],
		 SDL_Surface * screen, Sprites tableSurface[NBR_OF_IMAGES])
{
  if (screen == NULL || grid == NULL || screen == NULL)
    {
      fprintf (stderr, gettext ("init displayProgress failed: %s\n"),
	       SDL_GetError ());
      return EXIT_FAILURE;
    }

  SDL_Rect circlePos;
  /*place progress.png in the circle */
  circlePos.x = pref->x_menu + 16;
  circlePos.y = 5;

  /* progress.png is a small part of 360° cirle */
  int angle = (360 - goalLeft (pref, grid) * 360 / nbr_of_goals (pref, grid));
  //fprintf(stderr,"Angle is %d\n", angle);
  int i = 0;
  /* blit progress circle size image is 56*56px */
  for (i = 360; i >= angle; i = i - 5)
    {
      SDL_Surface *circle =
	rotozoomSurface (tableSurface[PROGRESS].image, i, 1.0, 1);

      circlePos.x = pref->x_menu + 16 + (56 - circle->w) / 2;
      circlePos.y = 5 + (56 - circle->h) / 2;
      if (angle != 360)
	{
	  SDL_BlitSurface (circle, NULL, screen, &circlePos);
	}
      SDL_FreeSurface (circle);
    }
  return EXIT_SUCCESS;
}

/* Victory or not ? */
int
levelFinished (S_preferences *pref,
               Square grid[pref->max_X_Blocks][pref->max_Y_Blocks])
{
  /* get info */
  int i = goalLeft (pref, grid);
  int j = nbr_of_goals (pref, grid);

  if (i == j)
    {
      return FINISH;
    }
  else
    {
      return NOT_FINISHED;
    }
}



/* display a congrats message to the winner */
int
displayCongrats (S_preferences *pref,
                 SDL_Surface * screen,
                 Sprites tableSurface[NBR_OF_IMAGES])
{
  SDL_Rect congratsPos;

  int width = 320, height = 120;

  /* add a background */
  congratsPos.x = (pref->window_width - width) / 2;
  congratsPos.y = (pref->window_height - height) / 2;
  SDL_BlitSurface (tableSurface[CONGRATS].image, NULL, screen, &congratsPos);

  /* setup font */
  TTF_Font *font = NULL;
  char fontPath[MAX_CARACT] = "";
  strcpy (fontPath, findFont ());

  const int fontCongratsSize = 30;
  font = TTF_OpenFont (fontPath, fontCongratsSize);
  SDL_Color fontColor = { 255, 255, 255, 255 };

  /* merge results */
  SDL_Surface *congratsMessage = NULL;
  SDL_Rect congratsMessagePos;
  char congratsMessageText[100] = "";
  strcpy (congratsMessageText, gettext ("CONGRATS!!!"));

  congratsMessage =
    TTF_RenderUTF8_Blended (font, congratsMessageText, fontColor);

  /* blit progress */
  congratsMessagePos.x = (pref->window_width - congratsMessage->w) / 2;
  congratsMessagePos.y = (pref->window_height - congratsMessage->h) / 2;
  SDL_BlitSurface (congratsMessage, NULL, screen, &congratsMessagePos);

  /* clean */
  SDL_FreeSurface (congratsMessage);
  TTF_CloseFont (font);
  font = NULL;

  return EXIT_SUCCESS;
}


/* Open And close the menu */
int
openCloseTheMenu(S_menuchoice *menuChoice){
		  if (menuChoice->open == 0)
            {
		    menuChoice->freeze = 1;
            menuChoice->open = 1;
            }
		  else{
		    menuChoice->freeze = 0;
            menuChoice->open = 0;
          }

        //fprintf (stderr, "change to freeze: %d open:%d\n", menuChoice->freeze, menuChoice->freeze);
        return 1;
}

/* Show menu List */
void
openMenu (S_preferences *pref,
          SDL_Surface * screen,
          Sprites tableSurface[NBR_OF_IMAGES],
	        S_Text tableTextSurface[NBR_OF_TAB][NBR_OF_TAB_LINE],
          S_LevelList * levelList,
          S_Menu gridMenu[pref->menu_X_Blocks][pref->menu_Y_Blocks])
{
  /* blit background */
  displayOpenMenuBackground (pref, screen, tableSurface, gridMenu);

  /* add the tab name above tab icons */
  SDL_Rect pos;
  /*blit text*/
  pos.x = pref->x_menu + SPRITE_SIZE;
  pos.y = 2 * SPRITE_SIZE + 10;
  size_t tab = gridMenu[menuChoice.xPos][1].text;
  SDL_BlitSurface (tableTextSurface[tab][0].image, NULL, screen, &pos);

  displaySubMenu (pref, screen, tableTextSurface, levelList);
  displayOverTextImage (pref, screen, tableSurface);

}

/* display Sub menu */
int
displaySubMenu (S_preferences * pref,
                SDL_Surface * screen,
                S_Text tableTextSurface[NBR_OF_TAB][NBR_OF_TAB_LINE],
                S_LevelList * levelList)
{
  /* blit text */
  //  fprintf (stderr, "menuChoice : %d\n", menuChoice.tab);
  SDL_Rect linePos;

  linePos.x = pref->x_menu + 40;

      /*Level Info */
      int i = 0;
      unsigned int sizeMax = 30;
      char nameLevel[MAX_CARACT] = "";
      strcpy (nameLevel, gettext ("Name: "));

      char nameFile[MAX_CARACT] = "";
      strcpy (nameFile, gettext ("File: "));

      char *pbuf;
      S_Level *actual = malloc (sizeof (*actual));
      if (actual == NULL)
	{
	  fprintf (stderr, gettext ("not enough memory"));
	  exit (EXIT_FAILURE);
	}

      actual = levelList->first;
      while (actual != NULL)
	{

	  if (i == pref->level)
	    {
	      /*trunk long name */
	      if (strlen (actual->name) > sizeMax)
		{
		  sprintf (nameLevel, "%s...",
			   strncat (nameLevel, actual->name, sizeMax));
		}
	      else
		{
		  sprintf (nameLevel, "%s", strcat (nameLevel, actual->name));
		}

	      /*trunk long file name */
	      pbuf = strpbrk (actual->fileName, "/");
	      if (strlen (pbuf) > sizeMax)
		{
		  sprintf (nameFile, gettext ("File: %s.."),
			   strncat (nameFile, pbuf, sizeMax));
		}
	      else
		{
		  sprintf (nameFile, gettext ("File: %s"), pbuf);
		}
	    }
	  actual = actual->next;
	  i++;
	}
      free (actual);

     size_t fontSize = 18, R = 255, G = 255, B = 255, A = 255;
     loadTextAsSurface (INFO,1, tableTextSurface, nameLevel,
		     fontSize, R, G, B, A);
     loadTextAsSurface (INFO,2, tableTextSurface, gettext ("Authors:"),
		     fontSize, R, G, B, A);
     loadTextAsSurface (INFO,3, tableTextSurface, nameFile,
		     fontSize, R, G, B, A);



   size_t nbr = 0, j = 0, tab = TAB_EMPTY;

   switch (menuChoice.tab){
   case M_INFO :
      nbr = MENU_MAX_INFO;
      tab = INFO;
      break;
   case M_ABOUT:
      nbr = MENU_MAX_ABOUT;
      tab = ABOUT;
      break;
   case M_SHORTCUTS:
      nbr = MENU_MAX_SHORTCUTS;
      tab = SHORTCUTS;
      break;
   case M_FILE:
      nbr = MENU_MAX_FILE;
      tab = FILES;
      break;
   case M_SETTINGS:
      nbr = MENU_MAX_SETTINGS;
      break;
   }
   nbr++;
   for (j = 1; j < nbr; j++ ){
          //fprintf(stderr, "line%zu: %zu\n",j,line[j]);
        if (tableTextSurface[tab][j].image != NULL){
        linePos.y = 3*SPRITE_SIZE + 10 + (j-1)*30;
        SDL_BlitSurface (tableTextSurface[tab][j].image, NULL, screen, &linePos);
        }
   }
  /*clean*/
  for (size_t i = 1; i <= 3; i++ ){
  SDL_FreeSurface (tableTextSurface[INFO][i].image);
  tableTextSurface[INFO][i].image = NULL;
  }
  return EXIT_SUCCESS;
}

/* Display the background menu */
void
displayOpenMenuBackground (S_preferences * pref,
                           SDL_Surface * screen,
			                     Sprites tableSurface[NBR_OF_IMAGES],
                           S_Menu gridMenu[pref->menu_X_Blocks][pref->menu_Y_Blocks])
{
  int i = 0;
  int xP = menuChoice.xPos , yP = menuChoice.yPos;
  int size = gridMenu[xP][yP].nbr_of_lines;


  SDL_Rect subMenuPos;
  subMenuPos.x = pref->x_menu+ 20;
  subMenuPos.y = SPRITE_SIZE;
  SDL_BlitSurface (tableSurface[MENU_RIBBON].image, NULL, screen,
		   &subMenuPos);
  SDL_BlitSurface (tableSurface[MENU_TOP_LIGHT].image, NULL, screen,
		   &subMenuPos);
  subMenuPos.y += SPRITE_SIZE;

  for (i = 1; i < size; i++)
    {
      SDL_BlitSurface (tableSurface[MENU_RIBBON].image, NULL, screen,
		       &subMenuPos);
      subMenuPos.y += SPRITE_SIZE;
    }
  /* add a bottom to the menu */
  SDL_BlitSurface (tableSurface[MENU_BOTTOM].image, NULL, screen,
		   &subMenuPos);


  /* Distribute littles squares behind the menu the menu */
  int x = 0, y = 0;
  SDL_Rect crossPos;
  crossPos.y = 60;
  for (y = 0; y < (size * 4 / 3); y++)
    {
      /*jump line for separator */
      if (y == 3)
	{
	  crossPos.y += 20;
	}
      crossPos.x = pref->x_menu + SPRITE_SIZE;
      for (x = 0; x < 11; x++)
	{
	  SDL_BlitSurface (tableSurface[MENU_SQUARE].image, NULL, screen,
			   &crossPos);
	  crossPos.x += 30;
	}
      crossPos.y += 30;

    }

  /* add a separator line */
  SDL_Rect sepPos;
  sepPos.x = pref->x_menu + 33;
  sepPos.y = 2*SPRITE_SIZE;
  SDL_BlitSurface (tableSurface[MENU_SEPARATOR].image, NULL, screen, &sepPos);
  sepPos.y = 3*SPRITE_SIZE;
  SDL_BlitSurface (tableSurface[MENU_SEPARATOR].image, NULL, screen, &sepPos);

  /*highlight the sub menuChoice*/

  SDL_Rect hlPos;
  hlPos.x = pref->x_menu + SPRITE_SIZE - 10;
  hlPos.y = 3*SPRITE_SIZE + 12 + menuChoice.sub*30;
  SDL_BlitSurface (tableSurface[MENU_HIGHLIGHT].image, NULL, screen, &hlPos);

  /* add highlight to tabs depending on the menuChoice*/
  SDL_Rect tabPos;
  tabPos.y = SPRITE_SIZE;
  tabPos.x = pref->x_menu +  menuChoice.xPos * SPRITE_SIZE + 3;
  SDL_BlitSurface (tableSurface[BUTTON_HIGHLIGHT].image, NULL, screen, &tabPos);

  /* add tab icons */

  SDL_Rect iconPos;
  iconPos.y = SPRITE_SIZE + 5;

  for(int y=0; y < pref->menu_Y_Blocks; ++y){
      for(int x=0; x< pref->menu_X_Blocks; ++x){
        if(gridMenu[x][y].type == TABS){
          iconPos.x = pref->x_menu + x*SPRITE_SIZE;
          // fprintf (stderr, "x: %d y:%d\n", iconPos.x,iconPos.y);
          SDL_BlitSurface (tableSurface[gridMenu[x][y].image].image, NULL, screen, &iconPos);
        }
      }
    }
}

/* Display pattern image over the text menu */
void
displayOverTextImage(S_preferences * pref,
                     SDL_Surface * screen,
		                 Sprites tableSurface[NBR_OF_IMAGES])
{
  int x = 0, y = 0, xSize = 360 / SPRITE_SIZE, ySize = 0;
  if (menuChoice.tab == M_ABOUT)
    {
      ySize = 8;
    }
  else
    {
      ySize = 7;
    }

  SDL_Rect Pos;
  Pos.x = pref->x_menu + 20;

  for (x = 0; x < xSize; x++)
    {
      Pos.y = SPRITE_SIZE + 10;
      for (y = 1; y < ySize; y++)
	{
	  SDL_BlitSurface (tableSurface[MENU_OVERTEXT].image, NULL, screen,
			   &Pos);
	  Pos.y += SPRITE_SIZE;
	}
      Pos.x += SPRITE_SIZE;
    }
}

/* Display buttons on the top bar*/
int
displayTopBarButtons (S_preferences *pref,
                      SDL_Surface * screen,
		                  Sprites tableSurface[NBR_OF_IMAGES],
                      S_Menu gridMenu[pref->menu_X_Blocks][pref->menu_Y_Blocks])
{
  SDL_Rect buttonsPos;
  int x = 0, y = 0;


  size_t  imageName = NO_IMAGE;
    for(y = 0; y < pref->menu_Y_Blocks; ++y){
        for(x = 0; x< pref->menu_X_Blocks; ++x){
          if (gridMenu[x][y].type == TOPBAR){
              imageName = gridMenu[x][y].image;

               buttonsPos.x = pref->x_menu + x * SPRITE_SIZE;
               buttonsPos.y = 0;

               //fprintf(stderr,"found x:%d->%d y:%d->%d\n", x, buttonsPos.x, y, buttonsPos.y);
               if (imageName != NO_IMAGE)
                   SDL_BlitSurface (tableSurface[imageName].image, NULL, screen,
                                    &buttonsPos);
           }
        }
    }
  return EXIT_SUCCESS;
}
#endif
