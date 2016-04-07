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

#include "const.h"
#include "game.h"

void readlevel(int map[][NBR_OF_BLOCKS], int level)
	{
	FILE* levelFile = NULL;
	//open file
	int c = 0, i = 0, j = 0, nb_line = 0;
	char lineLevel[NBR_OF_BLOCKS*NBR_OF_BLOCKS +1] = {0};
	levelFile = fopen("levels.txt", "r");
	  if (levelFile != NULL)
	    {

	      //find the number of lines (1 lines == 1 level)
	      while ((c = getc(levelFile)) != EOF)
		{
		  if ( c == '\n' )
		    {
		      nb_line++;
		    }
		}
	      fprintf(stderr, "Found %d levels\n", nb_line);
	//replace the cursor to the top
	      rewind(levelFile);
	//read lines
	      fgets(lineLevel, NBR_OF_BLOCKS * NBR_OF_BLOCKS +1, levelFile);
	//load each number
		for (i = 0; i < NBR_OF_BLOCKS, i++)//Height
		{
		  for (j= 0; j < NBR_OF_BLOCKS, j++)//Width
		    {
		      switch(lineLevel[i * NBR_OF_BLOCKS] + j)
			{
			case '0':
			  map[i][j] = 0;
			  break;
			case '1':
			  map[i][j] = 1;
			  break;
			case '2':
			  map[i][j] = 2 ;
			  break;
			case '3':
			  map[i][j] = 3;
			  break;
			case '4':
			  map[i][j] = 4;
			  break;
			}
		    }
		}
	fclose(levelFile);
	  	return 1;
	    }
	else
	  {
		fprintf(stderr, "Didn't load levels.txt : %s\n", SDL_GetError());
	    	return 0;
	  }
	}
#endif
