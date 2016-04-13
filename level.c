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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "const.h"
#include "game.h"

//read a level from the file
int readlevel(int map[][NBR_OF_BLOCKS], int level)
	{
	FILE* levelFile = NULL;
	//open file
	int c = 0, x = 0, y = 0, nb_line = 0;
	levelFile = fopen("levels.txt", "r");

	 //check if file exist
	  if (levelFile == NULL)
	  {
	    fprintf(stderr, "Can't open levels.txt : %s\n", SDL_GetError());
	    return 0;
	  }
	    fprintf(stderr, "levels.txt Opened: %s\n", SDL_GetError());
	//place the cursor depending of the level choosen
	  if( level == 0)
	    {
	    fseek(levelFile,0,SEEK_SET); //place cursor at the beginning
	    }
	  else
	    {
	      //read all caract before
	      int num_line = 0, caract = 0;
	       while(num_line < ((NBR_OF_BLOCKS +2)*level))
		 {
		     caract = fgetc(levelFile);
		     if(caract == '\n')
		     {
		     	num_line++;
		     }
		 }

	    }
	fprintf(stderr, "loading level: %d\t%s\n", level, SDL_GetError());
	//The cursor is supposed to be in the right position
	//read the level line by line
		int level_line = 0, num_caract = 0;
	    	while(level_line <= NBR_OF_BLOCKS)
		 {
		     //fprintf(stderr, "line : %d", level_line);
		     c = fgetc(levelFile);
		     if(c == '\n')
		     {
		     	level_line++;
		     }
		     else if(c == EOF)
		     {
		       level_line = NBR_OF_BLOCKS + 1;
		     }
		     else
		     {
			//show the readen level
		        fprintf(stderr, "%c", c);
		       	if (num_caract == (NBR_OF_BLOCKS -1))
			 {
		       	    fprintf(stderr, "\n");
			 }
		       	 if (num_caract == NBR_OF_BLOCKS)
			 {
			   num_caract = 0;
			 }
			//affect number to his destiny
		      	  switch(c)
					{
					case ' ':
					  map[num_caract][level_line] = GROUND;
					  break;
					case '#':
					  map[num_caract][level_line] = WALL;
					  break;
					case '$':
					  map[num_caract][level_line] = BOX ;
					  break;
					case '*':
					  map[num_caract][level_line] = BOX_OK ;
					  break;
					case '.':
					  map[num_caract][level_line] = GOAL;
					  break;
					case '@':
					  map[num_caract][level_line] = PLAYER;
					  break;
					}
			   num_caract++;
			 }
		 }
	  	fprintf(stderr, "Level Loaded %s\n", SDL_GetError());
		fclose(levelFile);
	}

//display level
void displayLevel(int map[][NBR_OF_BLOCKS], int mapGround[][NBR_OF_BLOCKS], SDL_Surface *screen, Sprites tableSurface[NBR_OF_IMAGES])
{
  	//goal
	SDL_Rect goalPos;

   	//blit surfaces depending of its destiny
	int x = 0, y = 0;
  	for (x = 0; x < NBR_OF_BLOCKS; x++)
	  {
		for (y = 0; y < NBR_OF_BLOCKS; y++)
		  {
		    //allow a random ground type to each block
			mapGround[x][y] = addRandomGround();

		    //blit all blocks depending on map types and mapGround types
		    switch(map[x][y])
		      {
		      case GROUND :
				blitGround(x,y,mapGround[x][y],screen,tableSurface);
			break;
		      case BOX :
				moveBox(x,y,map,STILL,screen, tableSurface); //see game.c
			break;
		      case BOX_OK :
				moveBox(x,y,map,STILL,screen, tableSurface); //see game.c
			break;
		      case WALL :
				blitWalls(x,y,map,screen,tableSurface);
			break;
		      case GOAL :
		      		goalPos.x = x * BOX_SIZE;
				goalPos.y = y * BOX_SIZE;
				blitGround(x,y,mapGround[x][y],screen,tableSurface);
				SDL_BlitSurface(tableSurface[GOAL_IMAGE].image, NULL, screen, &goalPos);
			break;
		      case PLAYER :
				blitGround(x,y, map[x][y], screen,tableSurface);
				blitPlayer(x,y, STILL , mapGround[x][y], screen, tableSurface);
			break;
			}
		  }
	  }
}


//how many level are in the file
int nbr_of_level()
	{
	int c = 0, nbrLevel = 1;
	//open file
	FILE* levelFile = NULL;
	levelFile = fopen("levels.txt", "r");
	  if (levelFile != NULL)
	    {
	      //find the number of lines (1 lines == 1 level)
	      while ((c = getc(levelFile)) != EOF)
		{
		  if ( c == '-' )
		    {
		      nbrLevel++;
		    }
		}
	    fclose(levelFile);
	    fprintf(stderr, "Found %d levels\n" ,nbrLevel);
	    return nbrLevel;
	    }
	else
	  {
		fprintf(stderr, "Can't open levels.txt : %s\n", SDL_GetError());
	    	return 0;
	  }
	}
#endif
