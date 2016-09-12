/*
 * C_SDL_Game is a simple SDL Game write to practice C Authors: kao_chen
 * Copyright (C) 2016 kao_chen <kaochen2[at]gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef SPRITES_C
#define SPRITES_C

#include "../inc/sprites.h"
#include <libintl.h>
#include <locale.h>

#include <sys/stat.h>

/* Load all sprites */
int
loadAllSprites (Sprites tableSurface[NBR_OF_IMAGES])
{

  /*read theme choice*/
  fprintf (stderr, "Loading Theme:\n");
  char themePath[MAX_CARACT] = "";
  char bufPath[MAX_CARACT] = "";
  getPrefChar (bufPath, "theme");

  sprintf (themePath, "img/%s/", bufPath);
  fprintf (stderr, "Theme: %s\n", themePath);

  /*test if folder exist*/
  struct stat  file_stat;
   if (stat(themePath,&file_stat) < 0)
     {
       printf("The theme folder %s does not exist\n", themePath);
       strcpy(themePath,"img/original/");
       fprintf (stderr, "Change theme folder to default: %s\n\n", themePath);
     }
   else
     {
     printf("The theme folder %s exist\n\n", themePath);
     }


  /* Player faces  */
  loadSprite (&tableSurface[PLAYER_FRONT], themePath,"player_front.png");
  loadSprite (&tableSurface[PLAYER_BACK], themePath,"player_back.png");
  loadSprite (&tableSurface[PLAYER_LEFT], themePath,"player_left.png");
  loadSprite (&tableSurface[PLAYER_RIGHT], themePath,"player_right.png");

  /* walls */
  loadSprite (&tableSurface[WALL1_IMAGE], themePath,"wall1.png");
  loadSprite (&tableSurface[WALL2_IMAGE], themePath,"wall2.png");
  loadSprite (&tableSurface[WALL3_IMAGE], themePath,"wall3.png");
  loadSprite (&tableSurface[WALL4_IMAGE], themePath,"wall4.png");
  loadSprite (&tableSurface[WALL_LEFT], themePath,"wallBorderLeft.png");
  loadSprite (&tableSurface[WALL_RIGHT], themePath,"wallBorderRight.png");
  loadSprite (&tableSurface[WALL_TOP], themePath,"wallBorderTop.png");
  loadSprite (&tableSurface[WALL_BOTTOM], themePath,"wallBorderBottom.png");

  /* walls */
  loadSprite (&tableSurface[CORNER_TR], themePath,"cornerTop-Right.png");
  loadSprite (&tableSurface[CORNER_TL], themePath,"cornerTop-Left.png");
  loadSprite (&tableSurface[CORNER_BR], themePath,"cornerBottom-Right.png");
  loadSprite (&tableSurface[CORNER_BL], themePath,"cornerBottom-Left.png");

  /* grounds */
  loadSprite (&tableSurface[GROUND1_IMAGE], themePath,"background1.bmp");
  loadSprite (&tableSurface[GROUND2_IMAGE], themePath,"background2.bmp");
  loadSprite (&tableSurface[GROUND3_IMAGE], themePath,"background3.bmp");

  /* boxes */
  loadSprite (&tableSurface[BOX_IMAGE], themePath,"box.png");
  loadSprite (&tableSurface[BOX_IMAGE_OK], themePath,"box_ok.png");

  /* goal */
  loadSprite (&tableSurface[GOAL_IMAGE], themePath,"goal.png");
  /* top bar */
  loadSprite (&tableSurface[PROGRESS], themePath,"progress.png");
  loadSprite (&tableSurface[MENU_BAR], themePath,"menu_bar.png");

  /* menu*/
  loadSprite (&tableSurface[MENU_RIBBON], themePath,"menu_ribbon.png");
  loadSprite (&tableSurface[MENU_BOTTOM], themePath,"menu_bottom.png");
  loadSprite (&tableSurface[MENU_TOP_LIGHT], themePath,"menu_top_light.png");
  loadSprite (&tableSurface[MENU_SQUARE], themePath,"menu_square.png");
  loadSprite (&tableSurface[MENU_SEPARATOR], themePath,"menu_separator.png");


  /* outside the wall */
  loadSprite (&tableSurface[OUTSIDE_IMAGE], themePath,"outside.png");
  loadSprite (&tableSurface[OUTSIDE2_IMAGE], themePath,"outside2.png");
  loadSprite (&tableSurface[OUTSIDE3_IMAGE], themePath,"outside3.png");
  loadSprite (&tableSurface[OUTSIDE4_IMAGE], themePath,"outside4.png");

  /* Congrats */
  loadSprite (&tableSurface[CONGRATS], themePath,"congrats_background.png");
  return EXIT_SUCCESS;
}


/* load a sprite as a surface */
void
loadSprite (Sprites * sprites, const char *themePath, const char *imageName)
{
  char filePath[MAX_CARACT] = "";
  strcpy(filePath, themePath);
  strcat(filePath,imageName);
  /* test if file exist */
  struct stat  file_stat;
   if (stat(filePath,&file_stat) < 0)
     {
       printf(gettext("the file %s does not exist\n"), filePath);
       strcpy(filePath,"img/original/");
       strcat(filePath,imageName);
       fprintf (stderr, gettext("Load default image instead: %s\n\n"), filePath);
     }
   else
     {
     printf(gettext("%s exist\n"), filePath);
     }

  sprites->image = IMG_Load (filePath);
  if (sprites->image == NULL)
    {
      fprintf (stderr, gettext("IMG_Load %s failed: %s\n"), filePath,
	       SDL_GetError ());
      exit (EXIT_FAILURE);
    }
}

/* free all surfaces */
void
freeSprites (Sprites tableSurface[NBR_OF_IMAGES])
{
  int i = 0;
  for (i = 0; i < NBR_OF_IMAGES; i++)
    {

      if (tableSurface[i].image != 0)
	    {
          SDL_FreeSurface (tableSurface[i].image);
	    }
    }
  fprintf (stderr, gettext("\tAll surfaces are free %s\n"), SDL_GetError ());
}


char *
findFont(void){
  char *ret = malloc (MAX_CARACT);
  char fontPath[MAX_CARACT] = "/usr/share/fonts/truetype/roboto/hinted/Roboto-Bold.ttf";
  struct stat  file_stat;
   if (stat(fontPath,&file_stat) < 0)
     {
       strcpy(fontPath,"/usr/share/fonts/truetype/roboto/Roboto-Bold.ttf");
         if (stat(fontPath,&file_stat) < 0)
           {
            fprintf(stderr, gettext("Roboto-Bold.ttf was not found. Install the fonts-roboto package\n"));
            exit (EXIT_FAILURE);
           }
     }
    strcpy(ret, fontPath);
    return ret;
    free(ret);
}
#endif
