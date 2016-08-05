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

#ifndef LEVEL_C
#define LEVEL_C

#include "../inc/level.h"


/* display level on the screen */
void displayLevel(Square grid[][getMax_Blocks()], SDL_Surface * screen,
		  Sprites tableSurface[NBR_OF_IMAGES])
{
    /* set a goal position */
    SDL_Rect pos;

    /* blit surfaces depending of its destiny */
    int x = 0, y = 0;
    /*first pass*/
    for (x = 0; x < getX_Blocks(); x++) {
	for (y = 0; y < getY_Blocks(); y++) {
		pos.x = x * SPRITE_SIZE;
		pos.y = y * SPRITE_SIZE;

      /* blit all blocks depending on grid subRole */
      switch (grid[x][y].subRole) {
        case GROUND1:
	    SDL_BlitSurface(tableSurface[GROUND1_IMAGE].image, NULL, screen,&pos);
	    break;
        case GROUND2:
	    SDL_BlitSurface(tableSurface[GROUND2_IMAGE].image, NULL, screen,&pos);
	    break;
        case GROUND3:
	    SDL_BlitSurface(tableSurface[GROUND3_IMAGE].image, NULL, screen,&pos);
        break;
	    case OUTSIDE:
		SDL_BlitSurface(tableSurface[OUTSIDE_IMAGE].image, NULL,screen, &pos);
		break;
	    case OUTSIDE2:
		SDL_BlitSurface(tableSurface[OUTSIDE2_IMAGE].image, NULL,screen, &pos);
		break;
	    case OUTSIDE3:
		SDL_BlitSurface(tableSurface[OUTSIDE3_IMAGE].image, NULL,screen, &pos);
		break;
        case OUTSIDE4:
		SDL_BlitSurface(tableSurface[OUTSIDE4_IMAGE].image, NULL,screen, &pos);
		break;
        case WALL1:
	    SDL_BlitSurface(tableSurface[WALL1_IMAGE].image, NULL, screen,&pos);
	    break;
        case WALL2:
	    SDL_BlitSurface(tableSurface[WALL2_IMAGE].image, NULL, screen,&pos);
	    break;
        case WALL3:
	    SDL_BlitSurface(tableSurface[WALL3_IMAGE].image, NULL, screen,&pos);
        break;
        case WALL4:
	    SDL_BlitSurface(tableSurface[WALL4_IMAGE].image, NULL, screen,&pos);
	    break;
	        }


        switch (grid[x][y].goalRole) {
        case GOAL:
        SDL_BlitSurface(tableSurface[GOAL_IMAGE].image, NULL,screen, &pos);
        break;
        }


	    /* blit all blocks depending on grid mainRole */
	    switch (grid[x][y].mainRole) {
	    case BOX:
        if(grid[x][y].goalRole == GOAL){
        SDL_BlitSurface(tableSurface[BOX_IMAGE_OK].image, NULL,screen, &pos);
          }
        else{
        SDL_BlitSurface(tableSurface[BOX_IMAGE].image, NULL,screen, &pos);
        }
		break;
	    case PLAYER:
        SDL_BlitSurface(tableSurface[GROUND1_IMAGE].image, NULL,screen, &pos);
		break;
       }

        switch (grid[x][y].playerRole) {
        case PLAYER_F:
	    SDL_BlitSurface(tableSurface[PLAYER_FRONT].image, NULL,screen, &pos);
	    break;
        case PLAYER_B:
	    SDL_BlitSurface(tableSurface[PLAYER_BACK].image, NULL,screen, &pos);
	    break;
        case PLAYER_L:
	    SDL_BlitSurface(tableSurface[PLAYER_LEFT].image, NULL,screen, &pos);
	    break;
        case PLAYER_R:
	    SDL_BlitSurface(tableSurface[PLAYER_RIGHT].image, NULL,screen, &pos);
	    break;
        }
    }

    }
     blitBorders(grid, screen,tableSurface);
     blitCorners(grid, screen,tableSurface);
}



/*get level name*/
int getLevelName(int levelChoice, S_LevelList * levelList, char * levelName)
{
    if (levelList == NULL) {
	return EXIT_FAILURE;
    }
    int i = 0;
    S_Level *actual = levelList->first;
    /* read the all chain list */
    while (actual != NULL) {
	/* try to find the nameLevel into the list */
	if (i == levelChoice) {
	    fprintf(stderr, "Actual name Level \"%s\"\n", actual->name);
       strcpy(levelName, actual->name);
	    break;
	}
	i++;
	actual = actual->next;
    }
 return EXIT_SUCCESS;
}

/*Found the level number with just the name*/
int findLevelNumber(S_LevelList * levelList, char * levelName)
{
    if (levelList == NULL) {
	    fprintf(stderr, "struct levelList does not exit");
    }
    int i = 0;
    S_Level *actual = levelList->first;
    /* read the all chain list */
    while (actual != NULL) {
	/* try to find the nameLevel into the list */
	if (!strcmp(actual->name,levelName)) {
	    fprintf(stderr, "I found the level %s : %d\n", actual->name, i);
       return i;
	    break;}
   else{
       	i++;
       	actual = actual->next;
   }
    }
 return 0;
}

/*get a levelchoice by reading the level name from the preference file*/
int readLevelFromSetting(S_LevelList * levelList)
{
    int i = 0, ret = 0;
    char levelName[MAX_CARACT]="";
    char *buf = getPrefAsChar("LevelName");
    strcpy(levelName, buf);
    fprintf(stderr, "First level to load is %s\n", levelName);

    if (levelList == NULL) {
	exit(EXIT_FAILURE);
    }
    S_Level *actual = levelList->first;
    while (actual != NULL) {
      if(strstr(actual->name, levelName) !=NULL){
          ret = i;
          break;
        }
      else{
        i++;
      }
	actual = actual->next;
    }
  fprintf(stderr,"levelchoice from file : %d\n ", ret);
  return ret;
}
#endif
