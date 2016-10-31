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
#include "../inc/menu_text.h"

#include <libintl.h>
#include <locale.h>

#include <sys/stat.h>

/* display Top Bar*/
int
displayTopBar (int levelNumber, SDL_Surface * screen,
	       Sprites tableSurface[NBR_OF_IMAGES],
	       S_Text tableTextSurface[NBR_OF_TEXT], S_LevelList * levelList,
	       Square grid[getMax_X_Blocks ()][getMax_Y_Blocks ()])
{
  /* first add background */
  if (backgroundTopBar (screen, tableSurface) == EXIT_FAILURE)
    {
      fprintf (stderr, gettext ("backgroundTopBar failed.\n"));
      return EXIT_FAILURE;
    }

  /* display the level number */
  if (levelMenu (levelNumber, screen, levelList) == EXIT_FAILURE)
    {
      fprintf (stderr, gettext ("levelMenu failed.\n"));
      return EXIT_FAILURE;
    }

  /* display the circle */
  if (displayProgress (grid, screen, tableSurface) == EXIT_FAILURE)
    {
      fprintf (stderr, gettext ("levelMenu failed.\n"));
      return EXIT_FAILURE;
    }

  /* display shortcut to open the menu */
  if (displayShortcut (screen, tableTextSurface) == EXIT_FAILURE)
    {
      fprintf (stderr, gettext ("levelMenu failed.\n"));
      return EXIT_FAILURE;
    }


  SDL_Rect facePos;
  facePos.x = menuPosX () + 24;
  facePos.y = 12;
  SDL_BlitSurface (tableSurface[PLAYER_FRONT].image, NULL, screen, &facePos);
  return EXIT_SUCCESS;

}

/* display background menu */
int
backgroundTopBar (SDL_Surface * screen, Sprites tableSurface[NBR_OF_IMAGES])
{
  if (screen == NULL || tableSurface == NULL)
    {
      fprintf (stderr, gettext ("init backgroundTopBar failed: %s\n"),
	       SDL_GetError ());
      return EXIT_FAILURE;
    }

  SDL_Rect menuPos;
  menuPos.x = menuPosX ();
  menuPos.y = 0;
  SDL_BlitSurface (tableSurface[MENU_BAR].image, NULL, screen, &menuPos);
  return EXIT_SUCCESS;

}


/* count how many goals are need to complete the level */
int
goalLeft (Square grid[getMax_X_Blocks ()][getMax_Y_Blocks ()])
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
nbr_of_goals (Square grid[getMax_X_Blocks ()][getMax_Y_Blocks ()])
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
int
displayProgress (Square grid[getMax_X_Blocks ()][getMax_Y_Blocks ()],
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
  circlePos.x = menuPosX () + 16;
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

      circlePos.x = menuPosX () + 16 + (56 - circle->w) / 2;
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
levelFinished (Square grid[getMax_X_Blocks ()][getMax_Y_Blocks ()])
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
	  S_Text tableTextSurface[NBR_OF_TEXT], S_LevelList * levelList,
	  int menuChoice, int levelChoice)
{
  /* blit background */
  displayOpenMenuBackground (screen, tableSurface, menuChoice);

  /* blit text */
  /* setup font */
  TTF_Font *font = NULL;
  char fontPath[MAX_CARACT] = "";
  strcpy (fontPath, findFont ());

  font = TTF_OpenFont (fontPath, 18);

  SDL_Color fontColor = { 255, 255, 255, 255 };

  SDL_Rect linePos;
  SDL_Surface *line1 = NULL;
  SDL_Surface *line2 = NULL;
  SDL_Surface *line3 = NULL;

  linePos.x = menuPosX () + 40;

  /* blit the text */
  line1 =
    TTF_RenderUTF8_Blended (font, gettext ("Current Level Infos"), fontColor);
  linePos.y = 60;
  SDL_BlitSurface (line1, NULL, screen, &linePos);

  line2 = TTF_RenderUTF8_Blended (font, gettext ("Shortcuts"), fontColor);
  linePos.y += 30;
  SDL_BlitSurface (line2, NULL, screen, &linePos);

  line3 = TTF_RenderUTF8_Blended (font, gettext ("About"), fontColor);
  linePos.y += 30;
  SDL_BlitSurface (line3, NULL, screen, &linePos);


  /*Blit highlight */
  SDL_Rect highlightPos;
  highlightPos.x = menuPosX () + 30;
  highlightPos.y = 63;

  if (menuChoice == 1)
    {
      highlightPos.y += 30;
    }
  else if (menuChoice == 2)
    {
      highlightPos.y += 60;
    }

  SDL_BlitSurface (tableSurface[MENU_HIGHLIGHT].image, NULL, screen,
		   &highlightPos);

  displaySubMenu (screen, tableTextSurface, levelList, menuChoice,
		  levelChoice);
  displayOverTextImage (screen, tableSurface, menuChoice);

  /* clean */
  SDL_FreeSurface (line1);
  SDL_FreeSurface (line2);
  SDL_FreeSurface (line3);
  TTF_CloseFont (font);
  font = NULL;
}

/* display Sub menu */
int
displaySubMenu (SDL_Surface * screen, S_Text tableTextSurface[NBR_OF_TEXT],
		S_LevelList * levelList, int menuChoice, int levelChoice)
{
  /* blit text */
  /* setup font */
  TTF_Font *font = NULL;
  char fontPath[MAX_CARACT] = "";
  strcpy (fontPath, findFont ());
  font = TTF_OpenFont (fontPath, 18);

  SDL_Color fontColor = { 255, 255, 255, 255 };
  SDL_Rect linePos;
  SDL_Surface *line01 = NULL;
  SDL_Surface *line02 = NULL;
  SDL_Surface *line03 = NULL;

  linePos.x = menuPosX () + 40;


  if (menuChoice == 0)
    {
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

      line01 = TTF_RenderUTF8_Blended (font, nameLevel, fontColor);
      linePos.y = 178;
      SDL_BlitSurface (line01, NULL, screen, &linePos);

      line02 = TTF_RenderUTF8_Blended (font, gettext ("Authors:"), fontColor);
      linePos.y += 30;
      SDL_BlitSurface (line02, NULL, screen, &linePos);

      line03 = TTF_RenderUTF8_Blended (font, nameFile, fontColor);
      linePos.y += 30;
      SDL_BlitSurface (line03, NULL, screen, &linePos);

    }

  /*Shortcuts */
  else if (menuChoice == 1)
    {
      linePos.y = 178;
      SDL_BlitSurface (tableTextSurface[SHORTCUTS1].textSurface, NULL, screen,
		       &linePos);

      linePos.y += 30;
      SDL_BlitSurface (tableTextSurface[SHORTCUTS2].textSurface, NULL, screen,
		       &linePos);

      linePos.y += 30;
      SDL_BlitSurface (tableTextSurface[SHORTCUTS3].textSurface, NULL, screen,
		       &linePos);

      linePos.y += 30;
      SDL_BlitSurface (tableTextSurface[SHORTCUTS4].textSurface, NULL, screen,
		       &linePos);

      linePos.y += 30;
      SDL_BlitSurface (tableTextSurface[SHORTCUTS5].textSurface, NULL, screen,
		       &linePos);
    }

  /*ABOUT */
  else if (menuChoice == 2)
    {
      linePos.y = 178;
      SDL_BlitSurface (tableTextSurface[ABOUT1].textSurface, NULL, screen,
		       &linePos);

      linePos.y += 30;
      SDL_BlitSurface (tableTextSurface[ABOUT2].textSurface, NULL, screen,
		       &linePos);
    }
  /* clean */
  SDL_FreeSurface (line01);
  SDL_FreeSurface (line02);
  SDL_FreeSurface (line03);
  TTF_CloseFont (font);
  font = NULL;
  return EXIT_SUCCESS;
}

/* Display the background menu */
void
displayOpenMenuBackground (SDL_Surface * screen,
			   Sprites tableSurface[NBR_OF_IMAGES],
			   int menuChoice)
{
  int i = 0, size = 0;
  switch (menuChoice)
    {
    case 0:
      size = 6;
      break;
    case 1:
      size = 7;
      break;
    case 2:
      size = 5;
      break;
    }

  SDL_Rect subMenuPos;
  subMenuPos.x = menuPosX () + 20;
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
  int x = 0, y = 0, start = menuPosX ();
  SDL_Rect crossPos;
  crossPos.y = 60;
  for (y = 0; y < (size * 4 / 3); y++)
    {
      /*jump line for separator */
      if (y == 3)
	{
	  crossPos.y += 20;
	}
      crossPos.x = start + SPRITE_SIZE;
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
  sepPos.x = start + 33;
  sepPos.y = 150;
  SDL_BlitSurface (tableSurface[MENU_SEPARATOR].image, NULL, screen, &sepPos);


}

/* Display pattern over the text menu */
void
displayOverTextImage (SDL_Surface * screen,
		      Sprites tableSurface[NBR_OF_IMAGES], int menuChoice)
{
  int x = 0, y = 0, xSize = 360 / SPRITE_SIZE, ySize = 0, start = menuPosX ();
  if (menuChoice == 1)
    {
      ySize = 8;
    }
  else
    {
      ySize = 7;
    }

  SDL_Rect Pos;
  Pos.x = start + 20;

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


#endif
