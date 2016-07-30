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



/* display menu */
void displayMenu(int levelNumber, SDL_Surface * menu,
		 Sprites tableSurface[NBR_OF_IMAGES],
		 S_LevelList * levelList,Square grid[][getMax_Blocks()])
{
    /* first add background */
    backgroundMenu(menu, tableSurface);
    /* display the level number */
    levelMenu(levelNumber, menu, levelList);
  	displayProgress(grid, menu, tableSurface);
    displayShortcut(menu);

    SDL_Rect facePos;
    facePos.x = menuPosX () + 24;
    facePos.y = 12;
  	 SDL_BlitSurface(tableSurface[PLAYER_FRONT].image, NULL, menu,
			&facePos);

}

/* display background menu */
void backgroundMenu(SDL_Surface * menu,
		    Sprites tableSurface[NBR_OF_IMAGES])
{
    SDL_Rect menuPos;
    menuPos.x = menuPosX();
    menuPos.y = 0;
	SDL_BlitSurface(tableSurface[MENU_BACK].image, NULL, menu,
			&menuPos);

}

/* display shortcut in the menu */
void displayShortcut(SDL_Surface * menu)
{
    /* setup font */
    TTF_Font *font = NULL;
    font = TTF_OpenFont("img/BABIRG__.TTF", 26);
    SDL_Color fontColor = { 255, 255, 255, 255 };
    SDL_Surface *shortCutText = NULL;
    shortCutText =
	TTF_RenderText_Blended(font,
			       "(M)",
			       fontColor);
    /* blit the text */
    SDL_Rect shortCutTextPos;
    shortCutTextPos.x = menuPosX() + (MENU_WIDTH - 2*SPRITE_SIZE - 4);
    shortCutTextPos.y = 10;
    SDL_BlitSurface(shortCutText, NULL, menu, &shortCutTextPos);

    /* clean */
    SDL_FreeSurface(shortCutText);
    TTF_CloseFont(font);
}

/* display the level number */
void levelMenu(int levelNumber, SDL_Surface * menu,
	       S_LevelList * levelList)
{
    /* setup font */
    TTF_Font *font = NULL;
    font = TTF_OpenFont("img/BABIRG__.TTF", 26);
    SDL_Color fontColor = { 255, 255, 255, 255 };

    /* count all levels from all files */
    int levelMax = getNbrOfLevels(levelList);

    /* indicate level nbr */
    SDL_Surface *levelText = NULL;
    char text[20] = "";
    sprintf(text, "Level: %d/%d", levelNumber, levelMax);
    levelText = TTF_RenderText_Blended(font, text, fontColor);

    /* blit the text */
    SDL_Rect levelTextPos;
    levelTextPos.x = menuPosX () + 150;
    levelTextPos.y = 10;
    SDL_BlitSurface(levelText, NULL, menu, &levelTextPos);

    /* clean */
    SDL_FreeSurface(levelText);
    TTF_CloseFont(font);
}

/* count how many goals are need to complete the level */
int goalLeft(Square grid[][getMax_Blocks()])
{
    int x = 0, y = 0, nbrOfBoxOk = 0;
    for (x = 0; x < getX_Blocks(); x++) {
	for (y = 0; y < getY_Blocks(); y++) {
	    if (grid[x][y].mainRole == BOX
		&& grid[x][y].objectType == GOAL)
		nbrOfBoxOk += 1;
	}
    }
    return nbrOfBoxOk;
}

/* count goals all ready achieve */
int nbr_of_goals(Square grid[][getMax_Blocks()])
{
    int x = 0, y = 0, nbrOfGoal = 0;
    for (x = 0; x < getX_Blocks(); x++) {
	for (y = 0; y < getY_Blocks(); y++) {
	    if (grid[x][y].objectType == GOAL)
		nbrOfGoal += 1;
	}
    }
    return nbrOfGoal;
}

/* Display Progress in the level */
void displayProgress(Square grid[][getMax_Blocks()], SDL_Surface * menu,
		     Sprites tableSurface[NBR_OF_IMAGES])
{
    SDL_Surface *circle = NULL;
    SDL_Rect circlePos;
    /*place progress.png in the circle*/
    circlePos.x = menuPosX () + 16;
    circlePos.y = 5;

    /* progress.png is a small part of 360° cirle*/
    int angle = (360 - goalLeft(grid)*360/nbr_of_goals(grid));
    //fprintf(stderr,"Angle is %d\n", angle);
    int i = 0;
    /* blit progress circle size image is 56*56px*/
  for(i=0;i<=angle;i = i+5){
    circle = rotozoomSurface(tableSurface[PROGRESS].image, i, 1.0, 1);
    circlePos.x = menuPosX () + 16 +(56-circle->w)/2;
    circlePos.y = 5 +(56-circle->h)/2;
    if(angle != 0){
    SDL_BlitSurface(circle, NULL, menu, &circlePos);
    SDL_FreeSurface(circle);
    }
  }

}

/* Victory or not ? */
int levelFinished(Square grid[][getMax_Blocks()], SDL_Surface * screen,
		  Sprites tableSurface[NBR_OF_IMAGES])
{

    /* get info */
    int i = goalLeft(grid);
    int j = nbr_of_goals(grid);

    if (i == j) {
	displayCongrats(screen, tableSurface);
	return FINISH;
    } else {
	return NOT_FINISHED;
    }
}



/* display a congrats message to the winner */
int displayCongrats(SDL_Surface * screen,
		    Sprites tableSurface[NBR_OF_IMAGES])
{
    SDL_Rect congratsPos;

    int width = 320, height = 120;

    /* add a background */
    congratsPos.x = ((getX_Blocks() * SPRITE_SIZE) - width) / 2;
    congratsPos.y = ((getY_Blocks() * SPRITE_SIZE) - height) / 2;
    SDL_BlitSurface(tableSurface[CONGRATS].image, NULL, screen,
		    &congratsPos);

    /* setup font */
    TTF_Font *font = NULL;
    const int fontCongratsSize = 50;
    font = TTF_OpenFont("img/BABIRG__.TTF", fontCongratsSize);
    SDL_Color fontColor = { 255, 255, 255, 255 };

    /* merge results */
    SDL_Surface *congratsMessage = NULL;
    SDL_Rect congratsMessagePos;
    char congratsMessageText[20] = "CONGRATS!!!";

    congratsMessage =
	TTF_RenderText_Blended(font, congratsMessageText, fontColor);


    /* blit progress */
    congratsMessagePos.x = (getWindow_width() - 250) / 2;
    congratsMessagePos.y = (getWindow_height() - fontCongratsSize) / 2;
    SDL_BlitSurface(congratsMessage, NULL, screen, &congratsMessagePos);

    /* clean */
    SDL_FreeSurface(congratsMessage);
    TTF_CloseFont(font);

    return EXIT_SUCCESS;
}

/* return the height of the menu in blocks*/
int menuHeight(void){
  int nbrOfBlocks = 1;
  return nbrOfBlocks;
}

/* menu position*/
int menuPosX(void){
  return ((getWindow_width() - MENU_WIDTH) /2);
}

/* Open the menu panel */
int openMenu(SDL_Surface * screen, Sprites tableSurface[NBR_OF_IMAGES])
{
    SDL_SaveBMP(screen, "/tmp/screenshot.bmp");
    SDL_Rect menuPos;
    menuPos.x = menuPosX() + 40;
    menuPos.y = 30;
    SDL_BlitSurface(tableSurface[CONGRATS].image, NULL, screen,
		    &menuPos);

    displayMenuList(screen, 0);
    displaySubMenu(screen,tableSurface, 0);

  return EXIT_SUCCESS;
}

/* display Sub menu */
int displaySubMenu(SDL_Surface * screen, Sprites tableSurface[NBR_OF_IMAGES], int subMenuChoice){
    SDL_Rect subMenuPos;
    subMenuPos.x = menuPosX() + 40;
    subMenuPos.y = 170;
    SDL_BlitSurface(tableSurface[CONGRATS].image, NULL, screen, &subMenuPos);
    fprintf(stderr, "levelChoice %d\n", subMenuChoice);
    return EXIT_SUCCESS;
}

/* Show menu List */
void displayMenuList(SDL_Surface * screen, int menuChoice)
{
    /* blit text */
    /* setup font */
    TTF_Font *font = NULL;
    font = TTF_OpenFont("img/BABIRG__.TTF", 26);
    SDL_Color fontColor = { 255, 255, 255, 255 };
    SDL_Surface *menuText = NULL;
    SDL_Rect menuTextPos;
    menuTextPos.x = menuPosX() + 60;

      /* blit the text */
    menuText = TTF_RenderText_Blended(font,"Current Level Infos", fontColor);
    menuTextPos.y = 60;
    SDL_BlitSurface(menuText, NULL, screen, &menuTextPos);

    menuText = TTF_RenderText_Blended(font,"Shortcuts", fontColor);
    menuTextPos.y += 30;
    SDL_BlitSurface(menuText, NULL, screen, &menuTextPos);

    menuText = TTF_RenderText_Blended(font,"About", fontColor);
    menuTextPos.y += 30;
    SDL_BlitSurface(menuText, NULL, screen, &menuTextPos);

   /*Blit point */
    /* setup font */
    TTF_Font *fontPoint = NULL;
    fontPoint = TTF_OpenFont("img/BABIRG__.TTF", 60);
    SDL_Color fontPointColor = { 255, 255, 255, 255 };
    SDL_Surface *point = NULL;
    SDL_Rect pointPos;
    pointPos.x = menuPosX() + 45;
    pointPos.y = 60;

    if(menuChoice == 1){
       pointPos.y += 30;
    }
    else if (menuChoice == 2){
       pointPos.y += 30;
    }

      point = TTF_RenderText_Blended(fontPoint,"*", fontPointColor);
      SDL_BlitSurface(point, NULL, screen, &pointPos);

    /* clean */
    SDL_FreeSurface(menuText);
    TTF_CloseFont(font);
    SDL_FreeSurface(point);
    TTF_CloseFont(fontPoint);
}

/* Open the menu panel */
int closeMenu(SDL_Surface * screen){
    SDL_Surface * image;
    image = IMG_Load("/tmp/screenshot.bmp");
    if (image == NULL) {
	      fprintf(stderr, "IMG_Load /tmp/screenshot.bmp failed: %s\n",SDL_GetError());
    }
    SDL_Rect posFill;
    posFill.x=0;
    posFill.y=0;

    SDL_BlitSurface(image, NULL, screen, &posFill);
    SDL_FreeSurface(image);
    remove("/tmp/screenshot.bmp");
    return EXIT_SUCCESS;
}

#endif
