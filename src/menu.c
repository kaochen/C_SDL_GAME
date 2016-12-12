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
S_Menu *
initGridMenu(void){

  S_Menu *gridMenu;
  gridMenu = malloc (MENU_LINE * sizeof (S_Menu));

   /* clean the array */
      for(size_t l = 0; l < MENU_LINE; l++){
      gridMenu[l] = (S_Menu) {.role=SUB_EMPTY, .type=TOPBAR, .text=SUB_EMPTY, .menu=MC_TOPBAR, .x=0, .y=0};
      }
   //place elements
   gridMenu[0] = (S_Menu) {.role=M_PREVIOUS, .type=TOPBAR, .text=SUB_EMPTY, .menu=MC_TOPBAR, .x=2, .y=0};
   gridMenu[1] = (S_Menu) {.role=M_NEXT, .type=TOPBAR,  .text=SUB_EMPTY, .menu=MC_TOPBAR, .x=7, .y=0};
   gridMenu[2] = (S_Menu) {.role=M_RESET, .type=TOPBAR, .text=SUB_EMPTY, .menu=MC_TOPBAR, .x=8, .y=0};
   gridMenu[3] = (S_Menu) {.role=M_BACKWARDS, .type=TOPBAR, .text=SUB_EMPTY, .menu=MC_TOPBAR, .x=9, .y=0};
   gridMenu[4] = (S_Menu) {.role=M_INFO, .type=OPENMENU, .text=INFO, .menu=MC_INFO, .x=1, .y=1};
   gridMenu[5] = (S_Menu) {.role=M_SHORTCUTS, .type=OPENMENU, .text=SHORTCUTS, .menu=MC_SHORTCUTS, .x=3, .y=1};
   gridMenu[6] = (S_Menu) {.role=M_ABOUT, .type=OPENMENU, .text=ABOUT, .menu=MC_ABOUT, .x=5, .y=1};
   gridMenu[7] = (S_Menu) {.role=M_FILE, .type=OPENMENU, .text=FILES, .menu=MC_FILE, .x=7, .y=1};

   return gridMenu;
}

/* display Top Bar*/
int
displayTopBar (S_preferences * pref,
	       SDL_Surface * screen,
	       Sprites tableSurface[NBR_OF_IMAGES],
	       S_LevelList * levelList,
	       Square grid[pref->max_X_Blocks][pref->max_Y_Blocks],
          S_Menu gridMenu[MENU_LINE])
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
	        S_Text tableTextSurface[NBR_OF_TEXT],
          S_LevelList * levelList,
          S_menuchoice menuChoice,
          S_Menu gridMenu[MENU_LINE])
{
  /* blit background */
  displayOpenMenuBackground (pref, screen, tableSurface, menuChoice);
  SDL_Rect pos;

     for (size_t l = 0; l < MENU_LINE; l++){

        if (gridMenu[l].type == OPENMENU && gridMenu[l].menu == menuChoice.tab){
           /*blit text*/
           pos.x = pref->x_menu + SPRITE_SIZE;
           pos.y = 2 * SPRITE_SIZE + 10;
           size_t surface = gridMenu[l].text;
           SDL_BlitSurface (tableTextSurface[surface].textSurface, NULL, screen, &pos);
        }
  }

  displaySubMenu (pref, screen, tableTextSurface, levelList, menuChoice);
  displayOverTextImage (pref, screen, tableSurface, menuChoice);

}

/* display Sub menu */
int
displaySubMenu (S_preferences * pref,
                SDL_Surface * screen,
                S_Text tableTextSurface[NBR_OF_TEXT],
                S_LevelList * levelList,
                S_menuchoice menuChoice)
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
     loadTextAsSurface (INFO1, tableTextSurface, nameLevel,
		     fontSize, R, G, B, A);
     loadTextAsSurface (INFO2, tableTextSurface, gettext ("Authors:"),
		     fontSize, R, G, B, A);
     loadTextAsSurface (INFO3, tableTextSurface, nameFile,
		     fontSize, R, G, B, A);



   size_t nbr = 0, j = 0, size = 10, line[size];
   for (j = 0; j < size; j++){
      line[size] = SUB_EMPTY;
   }
   j = 0;
   switch (menuChoice.tab){
   case MC_INFO :
      nbr = 3;
      line[0] = INFO1;
      line[1] = INFO2;
      line[2] = INFO3;
      break;
   case MC_ABOUT:
      nbr = 2;
      line[0] = ABOUT1;
      line[1] = ABOUT2;
      break;
   case MC_SHORTCUTS:
      nbr = 5;
      line[0] = SHORTCUTS1;
      line[1] = SHORTCUTS2;
      line[2] = SHORTCUTS3;
      line[3] = SHORTCUTS4;
      line[4] = SHORTCUTS5;
      break;
   }
   for (j = 0; j < nbr; j++ ){
            if (line[j] != SUB_EMPTY){
               //fprintf(stderr, "line%zu: %zu\n",j,line[j]);
               linePos.y = 3*SPRITE_SIZE + 10 + j*30;
               SDL_BlitSurface (tableTextSurface[line[j]].textSurface, NULL, screen, &linePos);
            }
   }
  /*clean*/
  SDL_FreeSurface (tableTextSurface[INFO1].textSurface);
  SDL_FreeSurface (tableTextSurface[INFO2].textSurface);
  SDL_FreeSurface (tableTextSurface[INFO3].textSurface);
  tableTextSurface[INFO1].textSurface = NULL;
  tableTextSurface[INFO2].textSurface = NULL;
  tableTextSurface[INFO3].textSurface = NULL;

  return EXIT_SUCCESS;
}

/* Display the background menu */
void
displayOpenMenuBackground (S_preferences * pref,
                           SDL_Surface * screen,
			                  Sprites tableSurface[NBR_OF_IMAGES],
			                  S_menuchoice menuChoice)
{
  int i = 0, size = 0;
  switch (menuChoice.tab)
    {
    case MC_INFO:
      size = 5;
      break;
    case MC_SHORTCUTS:
      size = 6;
      break;
    case MC_ABOUT:
      size = 4;
      break;
    case MC_FILE:
      size = 4;
      break;
    }

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

  /* add tabs */
  SDL_Rect tabPos;
  tabPos.x = pref->x_menu + 3*SPRITE_SIZE;
  tabPos.y = 2*SPRITE_SIZE - 25;
  for (size_t j = 0; j < 3; j++){
            SDL_BlitSurface (tableSurface[MENU_HIGHLIGHT].image, NULL, screen, &tabPos);
            tabPos.x += 2*SPRITE_SIZE;
  }
  /* add highlight to tabs depending on the menuChoice*/

  tabPos.x = pref->x_menu + SPRITE_SIZE + ((menuChoice.tab-1) * 2*SPRITE_SIZE) + 3;
  for (size_t j = 0; j < 2; j++){
            SDL_BlitSurface (tableSurface[MENU_HIGHLIGHT].image, NULL, screen, &tabPos);
            tabPos.x += 2*SPRITE_SIZE - 6;
  }

  /* add tab icons */

  SDL_Rect iconPos;
  iconPos.x = pref->x_menu + 3*SPRITE_SIZE/2;
  iconPos.y = SPRITE_SIZE + 5;
  SDL_BlitSurface (tableSurface[BUTTON_RESET].image, NULL, screen, &iconPos);
  iconPos.x += 2*SPRITE_SIZE;
  SDL_BlitSurface (tableSurface[BUTTON_ARROW_L].image, NULL, screen, &iconPos);
  iconPos.x += 2*SPRITE_SIZE;
  SDL_BlitSurface (tableSurface[BUTTON_ARROW_P].image, NULL, screen, &iconPos);
  iconPos.x += 2*SPRITE_SIZE;
  SDL_BlitSurface (tableSurface[BUTTON_FILE].image, NULL, screen, &iconPos);
}

/* Display pattern over the text menu */
void
displayOverTextImage(S_preferences * pref,
                     SDL_Surface * screen,
		               Sprites tableSurface[NBR_OF_IMAGES],
                     S_menuchoice menuChoice)
{
  int x = 0, y = 0, xSize = 360 / SPRITE_SIZE, ySize = 0;
  if (menuChoice.tab == MC_ABOUT)
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
displayTopBarButtons (S_preferences *pref,SDL_Surface * screen,
		      Sprites tableSurface[NBR_OF_IMAGES], S_Menu gridMenu[MENU_LINE])
{
  SDL_Rect buttonsPos;
  size_t  i = 0;

  for (size_t l = 0; l < MENU_LINE; l++){
   size_t  imageName = NO_IMAGE;
     switch (gridMenu[l].role)
     {
     case M_PREVIOUS :
            imageName = BUTTON_ARROW_L;
            break;
     case M_NEXT :
            imageName = BUTTON_ARROW_P;
        break;
     case M_RESET :
            imageName = BUTTON_RESET;
        break;
     case M_BACKWARDS :
            imageName = BUTTON_BACKWARDS;
        break;
     }
     buttonsPos.x = pref->x_menu + gridMenu[l].x * SPRITE_SIZE;
     buttonsPos.y = gridMenu[l].y + 1;
     if (imageName != NO_IMAGE)
         SDL_BlitSurface (tableSurface[imageName].image, NULL, screen,
                          &buttonsPos);
     i++;
  }
  return EXIT_SUCCESS;
}
#endif
