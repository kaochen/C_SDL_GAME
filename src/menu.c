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

#ifndef MENU_C
#define MENU_C

#include "../inc/menu.h"

#include <libintl.h>
#include <locale.h>

#include <sys/stat.h>

/* display Top Bar*/
void
displayTopBar (int levelNumber, SDL_Surface * screen,
	       Sprites tableSurface[NBR_OF_IMAGES],
	       S_LevelList * levelList, Square grid[][getMax_Blocks ()])
{
  /* first add background */
  backgroundTopBar (screen, tableSurface);
  /* display the level number */
  levelMenu (levelNumber, screen, levelList);
  displayProgress (grid, screen, tableSurface);
  displayShortcut (screen);

  SDL_Rect facePos;
  facePos.x = menuPosX () + 24;
  facePos.y = 12;
  SDL_BlitSurface (tableSurface[PLAYER_FRONT].image, NULL, screen, &facePos);

}

/* display background menu */
void
backgroundTopBar (SDL_Surface * screen, Sprites tableSurface[NBR_OF_IMAGES])
{
  SDL_Rect menuPos;
  menuPos.x = menuPosX ();
  menuPos.y = 0;
  SDL_BlitSurface (tableSurface[MENU_BAR].image, NULL, screen, &menuPos);

}

/* display shortcut in the menu */
void
displayShortcut (SDL_Surface * screen)
{
  /* setup font */
  TTF_Font *font = NULL;
  char fontPath[MAX_CARACT] = "";
  strcpy(fontPath,findFont());

  font = TTF_OpenFont (fontPath, 20);
  SDL_Color fontColor = { 255, 255, 255, 255 };
  SDL_Surface *shortCutText = NULL;
  shortCutText = TTF_RenderText_Blended (font, "(M)", fontColor);
  /* blit the text */
  SDL_Rect shortCutTextPos;
  shortCutTextPos.x = menuPosX () + (MENU_WIDTH - 2 * SPRITE_SIZE - 5);
  shortCutTextPos.y = (SPRITE_SIZE - shortCutText->h)/2;
  SDL_BlitSurface (shortCutText, NULL, screen, &shortCutTextPos);

  /* clean */
  SDL_FreeSurface (shortCutText);
  TTF_CloseFont (font);
font = NULL;
}

/* display the level number */
void
levelMenu (int levelNumber, SDL_Surface * screen, S_LevelList * levelList)
{
  /* setup font */
  TTF_Font *font = NULL;
  char fontPath[MAX_CARACT] = "";
  strcpy(fontPath,findFont());

  font = TTF_OpenFont (fontPath, 20);

  SDL_Color fontColor = { 255, 255, 255, 255 };

  /* count all levels from all files */
  int levelMax = getNbrOfLevels (levelList);

  /* indicate level nbr */
  SDL_Surface *levelText = NULL;
  char text[20] = "";
  sprintf (text, gettext("Level: %d/%d"), levelNumber, levelMax);
  levelText = TTF_RenderUTF8_Blended (font, text, fontColor);

  /* blit the text */
  SDL_Rect levelTextPos;
  levelTextPos.x = (getWindow_width ()  - levelText->w)/2;
  levelTextPos.y = (SPRITE_SIZE - levelText->h)/2;
  SDL_BlitSurface (levelText, NULL, screen, &levelTextPos);

  /* clean */
  SDL_FreeSurface (levelText);
  TTF_CloseFont (font);
  font = NULL;
}

/* count how many goals are need to complete the level */
int
goalLeft (Square grid[][getMax_Blocks ()])
{
  int x = 0, y = 0, nbrOfBoxOk = 0;
  for (x = 0; x < getX_Blocks (); x++)
    {
      for (y = 0; y < getY_Blocks (); y++)
	{
	  if (grid[x][y].mainRole == BOX && grid[x][y].goalRole == GOAL)
	    nbrOfBoxOk += 1;
	}
    }
  return nbrOfBoxOk;
}

/* count goals all ready achieve */
int
nbr_of_goals (Square grid[][getMax_Blocks ()])
{
  int x = 0, y = 0, nbrOfGoal = 0;
  for (x = 0; x < getX_Blocks (); x++)
    {
      for (y = 0; y < getY_Blocks (); y++)
	{
	  if (grid[x][y].goalRole == GOAL)
	    nbrOfGoal += 1;
	}
    }
  return nbrOfGoal;
}

/* Display Progress in the level */
void
displayProgress (Square grid[][getMax_Blocks ()], SDL_Surface * screen,
		 Sprites tableSurface[NBR_OF_IMAGES])
{
  SDL_Surface *circle = NULL;
  SDL_Rect circlePos;
  /*place progress.png in the circle */
  circlePos.x = menuPosX () + 16;
  circlePos.y = 5;

  /* progress.png is a small part of 360° cirle */
  int angle = (360 - goalLeft (grid) * 360 / nbr_of_goals (grid));
  //fprintf(stderr,"Angle is %d\n", angle);
  int i = 0;
  /* blit progress circle size image is 56*56px */
  for (i = 360; i >= angle; i = i - 5)
    {
      circle = rotozoomSurface (tableSurface[PROGRESS].image, i, 1.0, 1);
      circlePos.x = menuPosX () + 16 + (56 - circle->w) / 2;
      circlePos.y = 5 + (56 - circle->h) / 2;
      if (angle != 360)
	      {
	        SDL_BlitSurface (circle, NULL, screen, &circlePos);
	      }
    }
	SDL_FreeSurface (circle);
}

/* Victory or not ? */
int
levelFinished (Square grid[][getMax_Blocks ()])
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
displayCongrats (SDL_Surface * screen, Sprites tableSurface[NBR_OF_IMAGES])
{
  SDL_Rect congratsPos;

  int width = 320, height = 120;

  /* add a background */
  congratsPos.x = ((getX_Blocks () * SPRITE_SIZE) - width) / 2;
  congratsPos.y = ((getY_Blocks () * SPRITE_SIZE) - height) / 2;
  SDL_BlitSurface (tableSurface[CONGRATS].image, NULL, screen, &congratsPos);

  /* setup font */
  TTF_Font *font = NULL;
  char fontPath[MAX_CARACT] = "";
  strcpy(fontPath,findFont());

  const int fontCongratsSize = 30;
  font = TTF_OpenFont (fontPath, fontCongratsSize);
  SDL_Color fontColor = { 255, 255, 255, 255 };

  /* merge results */
  SDL_Surface *congratsMessage = NULL;
  SDL_Rect congratsMessagePos;
  char congratsMessageText[100] = "";
  strcpy(congratsMessageText, gettext("CONGRATS!!!"));

  congratsMessage =
    TTF_RenderUTF8_Blended (font, congratsMessageText, fontColor);

  /* blit progress */
  congratsMessagePos.x = (getWindow_width () - congratsMessage->w) / 2;
  congratsMessagePos.y = (getWindow_height () - congratsMessage->h) / 2;
  SDL_BlitSurface (congratsMessage, NULL, screen, &congratsMessagePos);

  /* clean */
  SDL_FreeSurface (congratsMessage);
  TTF_CloseFont (font);
  font = NULL;

  return EXIT_SUCCESS;
}

/* return the height of the menu in blocks*/
int
menuHeight (void)
{
  int nbrOfBlocks = 1;
  return nbrOfBlocks;
}

/* menu position*/
int
menuPosX (void)
{
  return ((getWindow_width () - MENU_WIDTH) / 2);
}



/* Show menu List */
void
openMenu (SDL_Surface * screen, Sprites tableSurface[NBR_OF_IMAGES],
	  S_LevelList * levelList, int menuChoice, int levelChoice)
{
  /* blit background */
  displayOpenMenuBackground(screen,tableSurface,menuChoice);

  /* blit text */
  /* setup font */
  TTF_Font *font = NULL;
  char fontPath[MAX_CARACT] = "";
  strcpy(fontPath,findFont());

  font = TTF_OpenFont (fontPath, 18);

  SDL_Color fontColor = { 255, 255, 255, 255 };
  SDL_Surface *menuText = NULL;
  SDL_Rect menuTextPos;
  menuTextPos.x = menuPosX () + 60;

  /* blit the text */
  menuText = TTF_RenderUTF8_Blended (font, gettext("Current Level Infos"), fontColor);
  menuTextPos.y = 60;
  SDL_BlitSurface (menuText, NULL, screen, &menuTextPos);
  menuText = NULL;

  menuText = TTF_RenderUTF8_Blended (font, gettext("Shortcuts"), fontColor);
  menuTextPos.y += 30;
  SDL_BlitSurface (menuText, NULL, screen, &menuTextPos);
  menuText = NULL;

  menuText = TTF_RenderUTF8_Blended (font, gettext("About"), fontColor);
  menuTextPos.y += 30;
  SDL_BlitSurface (menuText, NULL, screen, &menuTextPos);
  menuText = NULL;

  /*Blit point */
  /* setup font */
  TTF_Font *fontPoint = NULL;
  fontPoint = TTF_OpenFont (fontPath, 20);
  SDL_Color fontPointColor = { 255, 255, 255, 255 };
  SDL_Surface *point = NULL;
  SDL_Rect pointPos;
  pointPos.x = menuPosX () + 45;
  pointPos.y = 62;

  if (menuChoice == 1)
    {
      pointPos.y += 30;
    }
  else if (menuChoice == 2)
    {
      pointPos.y += 60;
    }

  point = TTF_RenderUTF8_Blended (fontPoint, "*", fontPointColor);
  SDL_BlitSurface (point, NULL, screen, &pointPos);

  displaySubMenu (screen, levelList, menuChoice, levelChoice);

  /* clean */
  SDL_FreeSurface (menuText);
  TTF_CloseFont (font);
  font = NULL;
  SDL_FreeSurface (point);
  TTF_CloseFont (fontPoint);
  fontPoint = NULL;
}

/* display Sub menu */
int
displaySubMenu (SDL_Surface * screen, S_LevelList * levelList, int menuChoice, int levelChoice)
{
  /* blit text */
  /* setup font */
  TTF_Font *font = NULL;
  char fontPath[MAX_CARACT] = "";
  strcpy(fontPath,findFont());
  font = TTF_OpenFont (fontPath, 18);

  SDL_Color fontColor = { 255, 255, 255, 255 };
  SDL_Surface *menuText = NULL;
  SDL_Rect menuTextPos;
  menuTextPos.x = menuPosX () + 60;


  if (menuChoice == 0)
    {
      /*Level Info */
      int i = 0;
      unsigned int sizeMax = 28;
      char nameLevel[MAX_CARACT] = "";
      strcpy(nameLevel, gettext("Name: "));

      char nameFile[MAX_CARACT] = "";
      strcpy(nameFile, gettext("File: "));

      char *pbuf;

      S_Level *actual = malloc (sizeof (*actual));
      actual = levelList->first;
      while (actual->name != NULL)
	        {
	          if (i == levelChoice)
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
		          sprintf (nameFile, gettext("File: %s.."), strncat(nameFile, pbuf, sizeMax));
		        }
	              else
		        {
		          sprintf (nameFile, gettext("File: %s"), pbuf);
		        }
	            }
	          actual = actual->next;
	          i++;
	        }
      free (actual);

      menuText = TTF_RenderUTF8_Blended (font, nameLevel, fontColor);
      menuTextPos.y = 180;
      SDL_BlitSurface (menuText, NULL, screen, &menuTextPos);

      menuText = TTF_RenderUTF8_Blended (font, gettext("Authors:"), fontColor);
      menuTextPos.y += 30;
      SDL_BlitSurface (menuText, NULL, screen, &menuTextPos);

      menuText = TTF_RenderUTF8_Blended (font, nameFile, fontColor);
      menuTextPos.y += 30;
      SDL_BlitSurface (menuText, NULL, screen, &menuTextPos);

    }

  /*Shortcuts */
  else if (menuChoice == 1)
    {
      menuText =
	TTF_RenderUTF8_Blended (font, gettext("m : open and close menu"), fontColor);
      menuTextPos.y = 180;
      SDL_BlitSurface (menuText, NULL, screen, &menuTextPos);

      menuText =
	TTF_RenderUTF8_Blended (font, gettext("n : next level"),
				fontColor);
      menuTextPos.y += 30;
      SDL_BlitSurface (menuText, NULL, screen, &menuTextPos);

      menuText =
	TTF_RenderUTF8_Blended (font, gettext("p : previous level"),
				fontColor);
      menuTextPos.y += 30;
      SDL_BlitSurface (menuText, NULL, screen, &menuTextPos);

      menuText =
	TTF_RenderUTF8_Blended (font,
				gettext("r : reset current level"),
				fontColor);
      menuTextPos.y += 30;
      SDL_BlitSurface (menuText, NULL, screen, &menuTextPos);

      menuText =
	TTF_RenderUTF8_Blended (font, gettext("q : quit game"),
				fontColor);
      menuTextPos.y += 30;
      SDL_BlitSurface (menuText, NULL, screen, &menuTextPos);
    }

  /* clean */
  SDL_FreeSurface (menuText);
  TTF_CloseFont (font);
  font = NULL;
  return EXIT_SUCCESS;
}

/* Display the background menu */
void
displayOpenMenuBackground(SDL_Surface * screen, Sprites tableSurface[NBR_OF_IMAGES], int menuChoice){
  int i = 0, size = 0;
  if (menuChoice == 1)
    {
      size = 7;
    }
   else
     {
       size = 6;
     }

  SDL_Rect subMenuPos;
  subMenuPos.x = menuPosX () + 25;
  subMenuPos.y = 40;
   SDL_BlitSurface (tableSurface[MENU_RIBBON].image, NULL, screen, &subMenuPos);
   SDL_BlitSurface (tableSurface[MENU_TOP_LIGHT].image, NULL, screen, &subMenuPos);
  subMenuPos.y += 40;

  for(i = 1; i < size; i++)
    {
      SDL_BlitSurface (tableSurface[MENU_RIBBON].image, NULL, screen, &subMenuPos);
      subMenuPos.y += 40;
   }
   /* add a bottom to the menu */
   SDL_BlitSurface (tableSurface[MENU_BOTTOM].image, NULL, screen, &subMenuPos);


  /* Distribute Cross over the menu */
  int x = 0, y = 0, start = menuPosX ();
  SDL_Rect  crossPos;
  crossPos.y = 60;
  for(y = 0; y < (size*4/3); y++)
    {
      /*jump line for separator*/
      if(y == 3)
        {
          crossPos.y += 20;
        }
         crossPos.x = start + 40;
           for(x = 0; x < 11; x++)
             {
               SDL_BlitSurface (tableSurface[MENU_SQUARE].image, NULL, screen, &crossPos);
               crossPos.x += 30;
             }
             crossPos.y += 30;

    }

  /* add a separator line */
  SDL_Rect sepPos;
  sepPos.x = start + 33;
  sepPos.y = 150;
  SDL_BlitSurface (tableSurface[MENU_SEPARATOR].image, NULL, screen, &sepPos);


}

#endif
