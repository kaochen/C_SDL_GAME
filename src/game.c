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

#ifndef GAME_C
#define GAME_C

#include "../inc/game.h"

#include <libintl.h>
#include <locale.h>

/* blit borders on walls */
int
blitBorders (Square grid[getMax_X_Blocks ()][getMax_Y_Blocks ()],
	     SDL_Surface * screen, Sprites tableSurface[NBR_OF_IMAGES])
{
   if (grid == NULL || screen == NULL || tableSurface == NULL)
    {
      fprintf (stderr, gettext("init blitBorders failed: %s\n"),
	       SDL_GetError ());
      return EXIT_FAILURE;
    }


  SDL_Rect wallPos;
  int x = 1, y = 1;
  for (y = 1; y < getY_Blocks () - 1; y++)
    {
      for (x = 1; x < getX_Blocks () - 1; x++)
	{
	  /* set position for wall blocks */
	  wallPos.x = x * SPRITE_SIZE;
	  wallPos.y = y * SPRITE_SIZE;

	  /* blit wall border, expect on window sides */
	  /* blit left border */
	  if (grid[x][y].mainRole == WALL)
	    {
	      if (grid[x - 1][y].mainRole != WALL)
		{
		  SDL_BlitSurface (tableSurface[WALL_LEFT].image, NULL,
				   screen, &wallPos);
		}
	      /* blit right border */
	      if (grid[x + 1][y].mainRole != WALL)
		{
		  SDL_BlitSurface (tableSurface[WALL_RIGHT].image, NULL,
				   screen, &wallPos);
		}
	      /* blit top border */
	      if (grid[x][y - 1].mainRole != WALL)
		{
		  SDL_BlitSurface (tableSurface[WALL_TOP].image, NULL, screen,
				   &wallPos);
		}
	      /* blit bottom border */
	      if (grid[x][y + 1].mainRole != WALL)
		{
		  SDL_BlitSurface (tableSurface[WALL_BOTTOM].image, NULL,
				   screen, &wallPos);
		}
	    }
	}
    }
  return EXIT_SUCCESS;
}

/* add a more ground choice */
int
randomGround (Square grid[getMax_X_Blocks ()][getMax_Y_Blocks ()])
{
  int x = 0, y = 0, randomNumber = 0;
  for (y = 0; y < getY_Blocks (); y++)
    {
      for (x = 0; x < getX_Blocks (); x++)
	{

	  randomNumber = random_number (0, 100);
	  if (grid[x][y].mainRole == GROUND)
	    {
	      if (randomNumber <= 60)
		{
		  grid[x][y].subRole = GROUND1;
		}
	      else if (randomNumber >= 90)
		{
		  grid[x][y].subRole = GROUND2;
		}
	      else
		{
		  grid[x][y].subRole = GROUND3;
		}
	    }
	}
    }
  return EXIT_SUCCESS;
}

/* add a more ground choice */
int
randomWall (Square grid[getMax_X_Blocks ()][getMax_Y_Blocks ()])
{
  int x = 0, y = 0, randomNumber = 0;
  for (y = 0; y < getY_Blocks (); y++)
    {
      for (x = 0; x < getX_Blocks (); x++)
	{

	  randomNumber = random_number (0, 100);
	  if (grid[x][y].mainRole == WALL)
	    {
	      if (randomNumber <= 60)
		{
		  grid[x][y].subRole = WALL1;
		}
	      else if (randomNumber >= 90)
		{
		  grid[x][y].subRole = WALL2;
		}
	      else if (randomNumber >= 70)
		{
		  grid[x][y].subRole = WALL3;
		}
	      else
		{
		  grid[x][y].subRole = WALL4;
		}
	    }

	}
    }
  return EXIT_SUCCESS;
}


/*change OUTSIDE subRole to get more choice between OUTSIDE, OUTSIDE2...*/
int
randomOutside (Square grid[getMax_X_Blocks ()][getMax_Y_Blocks ()])
{
  int x = 0, y = 0, randomNumber = 0;
  for (y = 0; y < getY_Blocks (); y++)
    {
      for (x = 0; x < getX_Blocks (); x++)
	{
	  if (grid[x][y].mainRole == OUTSIDE)
	    {
	      randomNumber = random_number (0, 100);
	      if (randomNumber <= 92)
		{
		  grid[x][y].subRole = OUTSIDE;
		}
	      else if (randomNumber <= 96)
		{
		  grid[x][y].subRole = OUTSIDE2;
		}
	      else if (randomNumber <= 98)
		{
		  grid[x][y].subRole = OUTSIDE3;
		}
	      else
		{
		  grid[x][y].subRole = OUTSIDE4;
		}
	    }
	}
    }
  return EXIT_SUCCESS;
}

/* random number between two numbers */
int
random_number (int min, int max)
{
  return rand () % (max - min + 1) + min;
}


/* blit corners */
int
blitCorners (Square grid[getMax_X_Blocks ()][getMax_Y_Blocks ()], SDL_Surface * screen,
	     Sprites tableSurface[NBR_OF_IMAGES])
{
  SDL_Rect pos;
  int x = 1, y = 1;
  for (y = 1; y < getY_Blocks (); y++)
    {
      for (x = 1; x < getX_Blocks (); x++)
	{
	  pos.x = x * SPRITE_SIZE;
	  pos.y = y * SPRITE_SIZE;
	  /*blit top right corners */
	  if (grid[x][y].mainRole == WALL
	      && grid[x][y - 1].mainRole == OUTSIDE
	      && grid[x + 1][y - 1].mainRole == OUTSIDE
	      && grid[x + 1][y].mainRole == OUTSIDE)
	    {
	      SDL_BlitSurface (tableSurface[CORNER_TR].image, NULL, screen,
			       &pos);
	      //fprintf(stderr, "found TOP_RIGHT %d/%d\n", x,y );
	    }
	  /*blit top left corners */
	  if (grid[x][y].mainRole == WALL
	      && grid[x][y - 1].mainRole == OUTSIDE
	      && grid[x - 1][y - 1].mainRole == OUTSIDE
	      && grid[x - 1][y].mainRole == OUTSIDE)
	    {
	      SDL_BlitSurface (tableSurface[CORNER_TL].image, NULL, screen,
			       &pos);
	      //fprintf(stderr, "found TOP_LEFT %d/%d\n", x,y );
	    }
	  /*blit bottom right corners */
	  if (grid[x][y].mainRole == WALL
	      && grid[x][y + 1].mainRole == OUTSIDE
	      && grid[x + 1][y + 1].mainRole == OUTSIDE
	      && grid[x + 1][y].mainRole == OUTSIDE)
	    {
	      SDL_BlitSurface (tableSurface[CORNER_BR].image, NULL, screen,
			       &pos);
	      //fprintf(stderr, "found BOTTOM_RIGHT %d/%d\n", x,y );
	    }
	  if (grid[x][y].mainRole == WALL
	      && grid[x][y + 1].mainRole == OUTSIDE
	      && grid[x - 1][y + 1].mainRole == OUTSIDE
	      && grid[x - 1][y].mainRole == OUTSIDE)
	    {
	      SDL_BlitSurface (tableSurface[CORNER_BL].image, NULL, screen,
			       &pos);
	      //fprintf(stderr, "found BOTTOM_LEFT %d/%d\n", x,y );
	    }
	}
    }
  return EXIT_SUCCESS;
}
#endif
