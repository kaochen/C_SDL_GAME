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

#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../inc/const.h"
#include "../inc/game.h"
#include "../inc/menu.h"
#include "../inc/level.h"
#include "../inc/slc.h"

/* display menu */
void displayMenu (int levelNumber, SDL_Surface * menu,
                  Sprites tableSurface[NBR_OF_IMAGES], S_LevelList *levelList)
{
   /* first add background */
   backgroundMenu (menu, tableSurface);
   /* display the level number */
   levelMenu (levelNumber, menu, tableSurface, levelList);
   displayShortcut (menu);
}

/* display background menu */
void backgroundMenu (SDL_Surface * menu, Sprites tableSurface[NBR_OF_IMAGES])
{
   SDL_Rect menuPos;
   menuPos.x = 0;
   menuPos.y = 0;
   int i = 0;
   for (i = 0; i < X_BLOCKS; i++)
   {
      SDL_BlitSurface (tableSurface[MENU_BACK].image, NULL, menu, &menuPos);
      menuPos.x += BOX_SIZE;
   }
}

/* display shortcut in the menu */
void displayShortcut (SDL_Surface * menu)
{
   /* setup font */
   TTF_Font *font = NULL;
   font = TTF_OpenFont ("img/BABIRG__.TTF", 26);
   SDL_Color fontColor = { 255, 255, 255 };
   SDL_Surface *shortCutText = NULL;
   shortCutText =
      TTF_RenderText_Blended (font, "N: next P: previous R: Reset Q: quit",
                              fontColor);
   /* blit the text */
   SDL_Rect shortCutTextPos;
   shortCutTextPos.x = BOX_SIZE * (X_BLOCKS - 7);
   shortCutTextPos.y = 10;
   SDL_BlitSurface (shortCutText, NULL, menu, &shortCutTextPos);

   /* clean */
   SDL_FreeSurface (shortCutText);
   TTF_CloseFont (font);
}

/* display the level number */
void levelMenu (int levelNumber, SDL_Surface * menu,
                Sprites tableSurface[NBR_OF_IMAGES], S_LevelList *levelList)
{
   /* setup font */
   TTF_Font *font = NULL;
   font = TTF_OpenFont ("img/BABIRG__.TTF", 26);
   SDL_Color fontColor = { 255, 255, 255 };

    /* count all levels from all files */
   int levelMax = getNbrOfLevels(levelList);

   /* indicate level nbr */
   SDL_Surface *levelText = NULL;
   char text[20] = "";
   sprintf (text, "Level: %d/%d", levelNumber, levelMax);
   levelText = TTF_RenderText_Blended (font, text, fontColor);

   /* blit the text */
   SDL_Rect levelTextPos;
   levelTextPos.x = 10;
   levelTextPos.y = 10;
   SDL_BlitSurface (levelText, NULL, menu, &levelTextPos);

   /* clean */
   SDL_FreeSurface (levelText);
   TTF_CloseFont (font);
}

/* count how many goals are need to complete the level */
int goalLeft (Square grid[][MAX_BLOCKS])
{
   int x = 0, y = 0, nbrOfBoxOk = 0;
   for (x = 0; x < X_BLOCKS; x++)
   {
      for (y = 0; y < Y_BLOCKS; y++)
      {
         if (grid[x][y].roleType == BOX && grid[x][y].objectType == GOAL)
            nbrOfBoxOk += 1;
      }
   }
   return nbrOfBoxOk;
}

/* count goals all ready achieve */
int nbr_of_goals (Square grid[][MAX_BLOCKS])
{
   int x = 0, y = 0, nbrOfGoal = 0;
   for (x = 0; x < X_BLOCKS; x++)
   {
      for (y = 0; y < Y_BLOCKS; y++)
      {
         if (grid[x][y].objectType == GOAL)
            nbrOfGoal += 1;
      }
   }
   return nbrOfGoal;
}

/* Display Progress in the level */
void displayProgress (Square grid[][MAX_BLOCKS], SDL_Surface * menu,
                      Sprites tableSurface[NBR_OF_IMAGES])
{
   SDL_Rect progressPos;
   progressPos.x = BOX_SIZE * 4;
   progressPos.y = 0;
   /* clean Background */
   int x = 0;
   for (x = 0; x < 3; x++)
   {
      SDL_BlitSurface (tableSurface[MENU_BACK].image, NULL, menu,
                       &progressPos);
      progressPos.x += BOX_SIZE;
   }

   /* setup font */
   TTF_Font *font = NULL;
   font = TTF_OpenFont ("img/BABIRG__.TTF", 26);
   SDL_Color fontColor = { 255, 255, 255 };
   /* get info */
   int i = goalLeft (grid);
   int j = nbr_of_goals (grid);
   /* merge results */
   SDL_Surface *progress = NULL;
   char progressText[20] = "";
   sprintf (progressText, "Goals: %d/%d", i, j);
   progress = TTF_RenderText_Blended (font, progressText, fontColor);

   /* blit progress */
   progressPos.x = BOX_SIZE * 4 ;
   progressPos.y = 10;
   SDL_BlitSurface (progress, NULL, menu, &progressPos);

   /* clean */
   SDL_FreeSurface (progress);
   TTF_CloseFont (font);
}

/* Victory or not ? */
int levelFinished (Square grid[][MAX_BLOCKS])
{
   if (nbr_of_goals (grid) == 0)
   {
      fprintf (stderr, "Congrats, you complete this level");
      return FINISH;
   }
   else
   {
      return NOT_FINISHED;
   }
}

#endif
