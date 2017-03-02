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

/* Init menu Choice */
void
menuChoiceInit(void)
{
/*init Menu Choice*/
  menuChoice.tabChoice = 1;
  menuChoice.lineChoice = MENU_OFFSET;
  menuChoice.nbrTabs = NBR_OF_TAB - 1;
  menuChoice.open = 0;
  menuChoice.freeze = 0;
  menuChoice.xPos = pref.xb_menu + 2;
  menuChoice.yPos = pref.xb_menu + 1;

  menuChoice.tab[0].nbrLines = 0;
  menuChoice.tab[0].name = TAB_EMPTY ;
  menuChoice.tab[0].image=NO_IMAGE;

  menuChoice.tab[1].nbrLines = MENU_MAX_INFO;
  menuChoice.tab[1].name = INFO;
  menuChoice.tab[1].image=BUTTON_LEVELS;

  menuChoice.tab[2].nbrLines = MENU_MAX_FILE;
  menuChoice.tab[2].name = FILES;
  menuChoice.tab[2].image=BUTTON_FILE;

  menuChoice.tab[3].nbrLines = MENU_MAX_SHORTCUTS;
  menuChoice.tab[3].name = SHORTCUTS;
  menuChoice.tab[3].image=BUTTON_SHORTCUTS;

  menuChoice.tab[4].nbrLines = MENU_MAX_SETTINGS;
  menuChoice.tab[4].name = SETTINGS;
  menuChoice.tab[4].image=BUTTON_SETTINGS;

  menuChoice.tab[5].nbrLines = MENU_MAX_ABOUT;
  menuChoice.tab[5].name = ABOUT;
  menuChoice.tab[5].image=BUTTON_ABOUT;

}

/* Init menu grid */
void
gridMenu_init(S_Menu gridMenu[pref.max_X_Blocks][pref.max_Y_Blocks])
{
   for(int y=0; y < pref.max_Y_Blocks; ++y)
      for(int x=0; x< pref.max_X_Blocks; ++x)
      gridMenu[x][y] = (S_Menu) {.role=M_EMPTY, .type=MAIN_WINDOW, .tab=0, .image=NO_IMAGE};

    //place elements
    gridMenu_initButtons(gridMenu);
    if (menuChoice.open == 1)
      {
      gridMenu_initTabs(gridMenu);
      }
}
/*Declare topbar buttons into the gridMenu*/
void
gridMenu_initButtons(S_Menu gridMenu[pref.max_X_Blocks][pref.max_Y_Blocks])
{
   for(int x=0; x< 10; ++x){
      gridMenu[pref.xb_menu + x][0] = (S_Menu) {.role=M_EMPTY, .type=TOPBAR, .tab=0, .image=NO_IMAGE};
   }

   gridMenu[pref.xb_menu + 2][0] = (S_Menu) {.role=M_PREVIOUS, .type=TOPBAR, .tab=0, .image=BUTTON_ARROW_L};
   gridMenu[pref.xb_menu + 7][0] = (S_Menu) {.role=M_NEXT, .type=TOPBAR,  .tab=0, .image=BUTTON_ARROW_P};
   gridMenu[pref.xb_menu + 8][0] = (S_Menu) {.role=M_RESET, .type=TOPBAR, .tab=0, .image=BUTTON_RESET};

    if(pref.achieved != 1)
      {
        gridMenu[pref.xb_menu + 9][0] = (S_Menu) {.role=M_CHECK, .type=TOPBAR, .tab=0, .image=BUTTON_CHECK};
      }
    else
      {
        gridMenu[pref.xb_menu + 9][0] = (S_Menu) {.role=M_CHECKED, .type=TOPBAR, .tab=0, .image=BUTTON_CHECKED};
      }

}

/*Declare tabs and its content into the gridMenu*/
void
gridMenu_initTabs(S_Menu gridMenu[pref.max_X_Blocks][pref.max_Y_Blocks])
{
   gridMenu[pref.xb_menu + 2][1] = (S_Menu) { .type=TABS, .tab=1};
   gridMenu[pref.xb_menu + 3][1] = (S_Menu) { .type=TABS, .tab=2};
   gridMenu[pref.xb_menu + 4][1] = (S_Menu) { .type=TABS, .tab=3};
   gridMenu[pref.xb_menu + 5][1] = (S_Menu) { .type=TABS, .tab=4};
   gridMenu[pref.xb_menu + 6][1] = (S_Menu) { .type=TABS, .tab=5};
};

/*Declare lines and its content into the gridMenu*/
void
gridMenu_initLines(S_Menu gridMenu[pref.max_X_Blocks][pref.max_Y_Blocks])
{
  //first clean old TABS_LINE
  for (int y = MENU_OFFSET; y < NBR_OF_TAB_LINE; y++)
    {
      for (int x = pref.xb_menu; x < pref.xb_menu + NBR_OF_TAB_LINE; x++)
        {
          gridMenu[x][y] = (S_Menu) {.type=MAIN_WINDOW, .tab= 0};
        }
    }
  //then write TABS_LINE to the grid depending on the menuchoice[current tab]nbrLines value
  for (int y = MENU_OFFSET; y < menuChoice.tab[menuChoice.tabChoice].nbrLines + MENU_OFFSET; y++)
    {
      for (int x = pref.xb_menu; x < pref.xb_menu + NBR_OF_TAB_LINE; x++)
        {
          gridMenu[x][y] = (S_Menu) {.type=TABS_LINE, .tab= menuChoice.tabChoice};
        }
    }
}
/* display Top Bar*/
int
displayTopBar (SDL_Surface * screen,
	            Sprites tableSurface[NBR_OF_IMAGES],
	            S_LevelList * levelList,
	            Square grid[pref.max_X_Blocks][pref.max_Y_Blocks],
              S_Menu gridMenu[pref.max_X_Blocks][pref.max_Y_Blocks])
{
  /* first add background */
  if (backgroundTopBar (screen, tableSurface) == EXIT_FAILURE)
    {
      fprintf (stderr, gettext ("backgroundTopBar failed.\n"));
      return EXIT_FAILURE;
    }

  /* display the level number */
  if (levelMenu (screen, levelList) == EXIT_FAILURE)
    {
      fprintf (stderr, gettext ("levelMenu failed.\n"));
      return EXIT_FAILURE;
    }

  /* display the buttons */
  if (displayTopBarButtons (screen, tableSurface, gridMenu) == EXIT_FAILURE)
    {
      fprintf (stderr, gettext ("displayTopBarButtons() failed.\n"));
      return EXIT_FAILURE;
    }

  /* display the circle */
  if (displayProgress (grid, screen, tableSurface) == EXIT_FAILURE)
    {
      fprintf (stderr, gettext ("displayProgress failed.\n"));
      return EXIT_FAILURE;
    }

  /* display the player into the circle */
  SDL_Rect facePos;
  facePos.x = pref.x_menu + 24;
  facePos.y = 12;
  SDL_BlitSurface (tableSurface[PLAYER_FRONT].image, NULL, screen, &facePos);
  return EXIT_SUCCESS;

}

/* display background of the top bar menu */
int
backgroundTopBar (SDL_Surface * screen,
                  Sprites tableSurface[NBR_OF_IMAGES])
{
  if (screen == NULL || tableSurface == NULL)
    {
      fprintf (stderr, gettext ("init backgroundTopBar failed: %s\n"),
	       SDL_GetError ());
      return EXIT_FAILURE;
    }

  SDL_Rect menuPos;
  menuPos.x = pref.x_menu;
  menuPos.y = 0;
  SDL_BlitSurface (tableSurface[MENU_BAR].image, NULL, screen, &menuPos);
  return EXIT_SUCCESS;
}


/* count how many goals are need to complete the level */
int
goalLeft (Square grid[pref.max_X_Blocks][pref.max_Y_Blocks])
{
  int x = 0, y = 0, nbrOfBoxOk = 0;
  for (x = 0; x < pref.max_X_Blocks; x++)
    {
      for (y = 0; y < pref.max_Y_Blocks; y++)
	{
	  if (grid[x][y].mainRole == BOX && grid[x][y].goalRole == GOAL)
	    nbrOfBoxOk += 1;
	}
    }
  return nbrOfBoxOk;
}

/* count goals all ready achieve */
int
nbr_of_goals (Square grid[pref.max_X_Blocks][pref.max_Y_Blocks])
{
  int x = 0, y = 0, nbrOfGoal = 0;
  for (x = 0; x < pref.x_Blocks; x++)
    {
      for (y = 0; y < pref.y_Blocks; y++)
	{
	  if (grid[x][y].goalRole == GOAL)
	    nbrOfGoal += 1;
	}
    }
  return nbrOfGoal;
}

/* Display Progress in the level */
int
displayProgress (Square grid[pref.max_X_Blocks][pref.max_Y_Blocks],
		            SDL_Surface * screen,
                Sprites tableSurface[NBR_OF_IMAGES])
{
  if (screen == NULL || grid == NULL || screen == NULL)
    {
      fprintf (stderr, gettext ("init displayProgress failed: %s\n"),
	       SDL_GetError ());
      return EXIT_FAILURE;
    }

  SDL_Rect circlePos;
  /*place progress.png in the circle */
  circlePos.x = pref.x_menu + 16;
  circlePos.y = 5;

  /* progress.png is a small part of 360° cirle */
  int angle = (360 - goalLeft (grid) * 360 / nbr_of_goals (grid));
  //fprintf(stderr,"Angle is %d\n", angle);
  int i = 0;
  /* blit progress circle size image is 56*56px */
  for (i = 360; i >= angle; i = i - 5)
    {
      SDL_Surface *circle =
	rotozoomSurface (tableSurface[PROGRESS].image, i, 1.0, 1);

      circlePos.x = pref.x_menu + 16 + (56 - circle->w) / 2;
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
levelFinished (Square grid[pref.max_X_Blocks][pref.max_Y_Blocks])
{
  /* get info */
  int i = goalLeft (grid);
  int j = nbr_of_goals (grid);

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
displayCongrats (SDL_Surface * screen,
                 Sprites tableSurface[NBR_OF_IMAGES])
{
  SDL_Rect congratsPos;

  /* add a background */
  congratsPos.x = (pref.window_width - tableSurface[CONGRATS].image->w) / 2;
  congratsPos.y = (pref.window_height - tableSurface[CONGRATS].image->h) / 2;
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
  congratsMessagePos.x = (pref.window_width - congratsMessage->w) / 2 + 30;
  congratsMessagePos.y = (pref.window_height - congratsMessage->h) / 2;
  SDL_BlitSurface (congratsMessage, NULL, screen, &congratsMessagePos);

  /* clean */
  SDL_FreeSurface (congratsMessage);
  TTF_CloseFont (font);
  font = NULL;

  return EXIT_SUCCESS;
}


/* Open And close the menu */
int
openCloseTheMenu(S_Menu gridMenu[pref.max_X_Blocks][pref.max_Y_Blocks]){
		  if (menuChoice.open == 0)
            {
		    menuChoice.freeze = 1;
            menuChoice.open = 1;
            }
		  else{
		    menuChoice.freeze = 0;
            menuChoice.open = 0;
          }
      gridMenu_init(gridMenu);

        //fprintf (stderr, "change to freeze: %d open:%d\n", menuChoice.freeze, menuChoice.freeze);
        return 1;
}

/* Show menu List */
void
openMenu (SDL_Surface * screen,
          Sprites tableSurface[NBR_OF_IMAGES],
	        S_Text tableTextSurface[NBR_OF_TAB][NBR_OF_TAB_LINE],
          S_LevelList * levelList,
          S_Menu gridMenu[pref.max_X_Blocks][pref.max_Y_Blocks])
{
  /* blit background */
  displayOpenMenuBackground (screen, tableSurface, gridMenu);

  /* add the tab name above tab icons */
  SDL_Rect pos;
  /*blit text*/
  pos.x = pref.x_menu + SPRITE_SIZE - 8;
  pos.y = 2 * SPRITE_SIZE + 10;
  size_t tabChoice = gridMenu[menuChoice.xPos][1].tab;
  size_t tab = menuChoice.tab[tabChoice].name;
  SDL_BlitSurface (tableTextSurface[tab][0].image, NULL, screen, &pos);

  displaySubMenu (screen, tableTextSurface, levelList);
  displayOverTextImage (screen, tableSurface);

}

/* display Sub menu */
int
displaySubMenu (SDL_Surface * screen,
                S_Text tableTextSurface[NBR_OF_TAB][NBR_OF_TAB_LINE],
                S_LevelList * levelList)
{
  /* blit text */
  //fprintf (stderr, "menuChoice : %d, %d\n", menuChoice.tabChoice, menuChoice.tab[menuChoice.tabChoice].nbrLines);

      /*Level Info */
      size_t sizeMax = 30;
      int i = 0;
      S_Level *current = malloc (sizeof (S_Level));
      getCurrentLevelInfos(levelList, current);

      char line[MENU_MAX_INFO + 1 ][MAX_CARACT] = { 0 };
      sprintf (line[1], gettext ("Name: %s"),current->name);
      sprintf (line[2], gettext ("File: %s"), current->fileName);
      sprintf (line[3], gettext ("Author: %s"), current->author);
      sprintf (line[4], gettext ("Level size: %dx%d"), current->width, current->height);
      free(current);

     size_t fontSize = 20, R = 255, G = 255, B = 255, A = 255;
      for (i = 1; i <= MENU_MAX_INFO; i++)
          {
           trunkLongChar(sizeMax, line[i]);
           loadTextAsSurface (INFO,i, tableTextSurface, line[i],
		     fontSize, R, G, B, A);
          }


  /*print lines into tabs*/
   int tabChoice = menuChoice.tabChoice, nbr = menuChoice.tab[tabChoice].nbrLines + 1;
   int tabName = menuChoice.tab[tabChoice].name;
   SDL_Rect linePos;
   linePos.x = pref.x_menu + 40;

   for (i = 1; i < nbr; i++ ){
        if (tableTextSurface[tabName][i].image != NULL){
        linePos.y = 3*SPRITE_SIZE + 10 + (i-1)*SPRITE_SIZE;
        SDL_BlitSurface (tableTextSurface[tabName][i].image, NULL, screen, &linePos);
        }
   }
  /*clean*/
  for ( i = 1; i <= MENU_MAX_INFO; i++ ){
  SDL_FreeSurface (tableTextSurface[INFO][i].image);
  tableTextSurface[INFO][i].image = NULL;
  }
  return EXIT_SUCCESS;
}

/* Display the background menu */
void
displayOpenMenuBackground (SDL_Surface * screen,
			                     Sprites tableSurface[NBR_OF_IMAGES],
                           S_Menu gridMenu[pref.max_X_Blocks][pref.max_Y_Blocks])
{
  int i = 0;
  int size = menuChoice.tab[menuChoice.tabChoice].nbrLines + 2;

  SDL_Rect subMenuPos;
  subMenuPos.x = pref.x_menu+ 20;
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
      crossPos.x = pref.x_menu + SPRITE_SIZE;
      for (x = 0; x < 11; x++)
	{
	  SDL_BlitSurface (tableSurface[MENU_SQUARE].image, NULL, screen,
			   &crossPos);
	  crossPos.x += 30;
	}
      crossPos.y += 30;

    }

  /* add background to the menu lines */
  SDL_Rect h_linePos;
  for (i = 2; i < size + 1; i++)
    {
       h_linePos.x = pref.x_menu + 30;
       h_linePos.y = i*SPRITE_SIZE;
       SDL_BlitSurface (tableSurface[MENU_H_LINE].image, NULL, screen, &h_linePos);
    }

  /*highlight the sub menuChoice*/

  SDL_Rect hlPos;
  hlPos.x = pref.x_menu + SPRITE_SIZE;
  hlPos.y = (menuChoice.lineChoice)*SPRITE_SIZE + 1;
       for (int j = 0; j < 8; j++){
          SDL_BlitSurface (tableSurface[MENU_HIGHLIGHT].image, NULL, screen, &hlPos);
          hlPos.x += SPRITE_SIZE;
       }


  /* add tab icons */
  SDL_Rect iconPos;
  iconPos.y = SPRITE_SIZE + 2;

  for(int y=0; y < pref.max_Y_Blocks; ++y){
      for(int x=0; x< pref.max_X_Blocks; ++x){
        if(gridMenu[x][y].type == TABS){
          iconPos.x = x*SPRITE_SIZE;
          // fprintf (stderr, "x: %d y:%d\n", iconPos.x,iconPos.y);
          int tabImage = menuChoice.tab[gridMenu[x][y].tab].image;
          SDL_BlitSurface (tableSurface[tabImage].image, NULL, screen, &iconPos);
        }
      }
    }
  // highlight selected tab //
  SDL_Rect highlightPos;
  highlightPos.y = SPRITE_SIZE;
  highlightPos.x = pref.x_menu + (1 + menuChoice.tabChoice)*SPRITE_SIZE;
  SDL_BlitSurface (tableSurface[MENU_HIGHLIGHT].image, NULL, screen, &highlightPos);
}

/* Display pattern image over the text menu */
void
displayOverTextImage(SDL_Surface * screen,
		                 Sprites tableSurface[NBR_OF_IMAGES])
{
  int x = 0, y = 0, xSize = 360 / SPRITE_SIZE, ySize = menuChoice.tab[menuChoice.tabChoice].nbrLines + 3;

  SDL_Rect Pos;
  Pos.x = pref.x_menu + 20;
  Pos.y = SPRITE_SIZE;
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
displayTopBarButtons (SDL_Surface * screen,
		                  Sprites tableSurface[NBR_OF_IMAGES],
                      S_Menu gridMenu[pref.max_X_Blocks][pref.max_Y_Blocks])
{
  SDL_Rect buttonsPos;
  int x = 0, y = 0;

  size_t  imageName = NO_IMAGE;
    for(y = 0; y < pref.max_Y_Blocks; ++y){
        for(x = 0; x< pref.max_X_Blocks; ++x){
          if (gridMenu[x][y].type == TOPBAR && gridMenu[x][y].role != M_EMPTY ){
              imageName = gridMenu[x][y].image;

               buttonsPos.x = x * SPRITE_SIZE;
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
