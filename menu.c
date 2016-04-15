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

#include "const.h"
#include "game.h"
#include "menu.h"
#include "level.h"

//display menu on top of the screen
void displayMenu(int levelNumber, SDL_Surface *menu, Sprites tableSurface[NBR_OF_IMAGES],int map[][NBR_OF_BLOCKS])
{
	//first add background:
	backgroundMenu(menu, tableSurface);
 	 //display the level number
	levelMenu(levelNumber,menu,tableSurface);
	displayShortcut(menu);
}

//display menu on top of the screen
void backgroundMenu(SDL_Surface *menu, Sprites tableSurface[NBR_OF_IMAGES])
{
  	SDL_Rect menuPos;
  	menuPos.x = 0;
  	menuPos.y = 0;
  	int i = 0;
  	for (i = 0; i < NBR_OF_BLOCKS; i++)
	  {
		SDL_BlitSurface(tableSurface[MENU_BACK].image, NULL,menu,&menuPos);
		menuPos.x += BOX_SIZE;
	   }
}

//display shortcut in the menu
void displayShortcut( SDL_Surface *menu)
{
  //setup font
  TTF_Font *font = NULL;
  font = TTF_OpenFont("img/BABIRG__.TTF", 26);
  SDL_Color fontColor = {255,255,255};
  SDL_Surface *shortCutText = NULL;
  shortCutText = TTF_RenderText_Blended(font, "N: next P: previous R: Reset Q: quit", fontColor);
  //blit the text
  SDL_Rect shortCutTextPos;
  shortCutTextPos.x = BOX_SIZE*4;
  shortCutTextPos.y = 10;
  SDL_BlitSurface(shortCutText, NULL, menu, &shortCutTextPos);

    //clean
  SDL_FreeSurface(shortCutText);
  TTF_CloseFont(font);
}


//display the level number
void levelMenu(int levelNumber, SDL_Surface *menu, Sprites tableSurface[NBR_OF_IMAGES])
{
  //manage font
  TTF_Font *font = NULL;
  font = TTF_OpenFont("img/BABIRG__.TTF", 26);
  SDL_Color fontColor = {255,255,255};

  //get info
  int levelMax = 0;
  levelMax = nbr_of_level();
  //indicate level nbr
  SDL_Surface *levelText = NULL;
  char text[20] = "";
  sprintf(text, "Level: %d/%d", levelNumber, levelMax);
  levelText = TTF_RenderText_Blended(font, text, fontColor);

  //blit the text
  SDL_Rect levelTextPos;
  levelTextPos.x = BOX_SIZE*(NBR_OF_BLOCKS-3);
  levelTextPos.y = 10;
  SDL_BlitSurface(levelText, NULL, menu, &levelTextPos);

  //clean
  SDL_FreeSurface(levelText);
  TTF_CloseFont(font);
}

//count how many goals are need to complete the level
int goalLeft(int map[][NBR_OF_BLOCKS])
{
  int x = 0, y = 0, nbrOfBoxOk = 0;
  for (x=0; x < NBR_OF_BLOCKS; x++)
    {
      for (y=0; y < NBR_OF_BLOCKS; y++)
    	{
	  if(map[x][y] == BOX_OK)
	    nbrOfBoxOk +=1;
    	}
    }
  return nbrOfBoxOk;
}

//count how many goals left to complete the level
int nbr_of_goals(int map[][NBR_OF_BLOCKS])
{
  int x = 0, y = 0, nbrOfGoal = 0;
  for (x=0; x < NBR_OF_BLOCKS; x++)
    {
      for (y=0; y < NBR_OF_BLOCKS; y++)
    	{
	  if(map[x][y] == GOAL)
	    nbrOfGoal +=1;
    	}
    }
  return nbrOfGoal;
}

//display Progress In The Level
void displayProgress(int map[][NBR_OF_BLOCKS], SDL_Surface *menu, Sprites tableSurface[NBR_OF_IMAGES])
{
   SDL_Rect progressPos;
  progressPos.x = 0;
  progressPos.y = 0;
  //clean Background
  int x = 0;
  for (x = 0; x < 3; x++)
    {
  	SDL_BlitSurface(tableSurface[MENU_BACK].image, NULL, menu, &progressPos);
        progressPos.x += BOX_SIZE;
    }


   //manage font
  TTF_Font *font = NULL;
  font = TTF_OpenFont("img/BABIRG__.TTF", 26);
  SDL_Color fontColor = {255,255,255};
  //get info
  int i = goalLeft(map);
  int j = i + nbr_of_goals(map);
  //merge results
  SDL_Surface *progress = NULL;
  char progressText[20] = "";
  sprintf(progressText, "Goals: %d/%d", i, j);
  progress = TTF_RenderText_Blended(font, progressText, fontColor);

  //blit progress
  progressPos.x = 10;
  progressPos.y = 10;
  SDL_BlitSurface(progress, NULL, menu, &progressPos);

    //clean
  SDL_FreeSurface(progress);
  TTF_CloseFont(font);
}


//Victory or not ?
int levelFinished(int map[][NBR_OF_BLOCKS])
{
  if (nbr_of_goals(map)==0)
    {
      fprintf(stderr, "Congrats, you complete this level");
      return FINISH;
    }
  else
    {
      return NOT_FINISHED;
    }
}

      #endif

