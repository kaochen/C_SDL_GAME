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

/*This file contains all that concern the image files and sprites management.*/

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



  /* Player faces  */
  loadSprite (&tableSurface[PLAYER_FRONT], pref.themePath, "player_front.png");
  loadSprite (&tableSurface[PLAYER_BACK], pref.themePath, "player_back.png");
  loadSprite (&tableSurface[PLAYER_LEFT], pref.themePath, "player_left.png");
  loadSprite (&tableSurface[PLAYER_RIGHT], pref.themePath, "player_right.png");

  /* Target to blit over ground */
  loadSprite (&tableSurface[TARGET_IMAGE], pref.themePath, "target.png");

  /* walls */
  loadSprite (&tableSurface[WALL1_IMAGE], pref.themePath, "wall1.png");
  loadSprite (&tableSurface[WALL2_IMAGE], pref.themePath, "wall2.png");
  loadSprite (&tableSurface[WALL3_IMAGE], pref.themePath, "wall3.png");
  loadSprite (&tableSurface[WALL4_IMAGE], pref.themePath, "wall4.png");
  loadSprite (&tableSurface[WALL_LEFT], pref.themePath, "wallBorderLeft.png");
  loadSprite (&tableSurface[WALL_RIGHT], pref.themePath, "wallBorderRight.png");
  loadSprite (&tableSurface[WALL_TOP], pref.themePath, "wallBorderTop.png");
  loadSprite (&tableSurface[WALL_BOTTOM], pref.themePath, "wallBorderBottom.png");

  /* walls */
  loadSprite (&tableSurface[CORNER_TR], pref.themePath, "cornerTop-Right.png");
  loadSprite (&tableSurface[CORNER_TL], pref.themePath, "cornerTop-Left.png");
  loadSprite (&tableSurface[CORNER_BR], pref.themePath, "cornerBottom-Right.png");
  loadSprite (&tableSurface[CORNER_BL], pref.themePath, "cornerBottom-Left.png");

  /* grounds */
  loadSprite (&tableSurface[GROUND1_IMAGE], pref.themePath, "tile1.png");
  loadSprite (&tableSurface[GROUND2_IMAGE], pref.themePath, "tile2.png");
  loadSprite (&tableSurface[GROUND3_IMAGE], pref.themePath, "tile3.png");

  /* boxes */
  loadSprite (&tableSurface[BOX_IMAGE], pref.themePath, "box.png");
  loadSprite (&tableSurface[BOX_IMAGE_OK], pref.themePath, "box_ok.png");

  /* goal */
  loadSprite (&tableSurface[GOAL_IMAGE], pref.themePath, "tile_goal.png");
  /* top bar */
  loadSprite (&tableSurface[PROGRESS], pref.themePath, "progress.png");
  loadSprite (&tableSurface[MENU_BAR], pref.themePath, "menu_bar.png");

  /* menu */
  loadSprite (&tableSurface[MENU_RIBBON], pref.themePath, "menu_ribbon.png");
  loadSprite (&tableSurface[MENU_BOTTOM], pref.themePath, "menu_bottom.png");
  loadSprite (&tableSurface[MENU_TOP_LIGHT], pref.themePath, "menu_top_light.png");
  loadSprite (&tableSurface[MENU_SQUARE], pref.themePath, "menu_square.png");
  loadSprite (&tableSurface[MENU_SEPARATOR], pref.themePath, "menu_separator.png");
  loadSprite (&tableSurface[MENU_H_LINE], pref.themePath, "menu_h_line.png");
  loadSprite (&tableSurface[MENU_V_LINE], pref.themePath, "menu_v_line.png");
  loadSprite (&tableSurface[MENU_CIRCLE], pref.themePath, "menu_circle.png");
  loadSprite (&tableSurface[MENU_CURVE], pref.themePath, "menu_curve.png");
  loadSprite (&tableSurface[MENU_OVERTEXT], pref.themePath, "menu_overText.png");
  loadSprite (&tableSurface[MENU_HIGHLIGHT], pref.themePath, "menu_highlighter.png");

  /* buttons */
  loadSprite (&tableSurface[BUTTON_ARROW_L], pref.themePath, "button_arrow_l.png");
  loadSprite (&tableSurface[BUTTON_ARROW_P], pref.themePath, "button_arrow_p.png");
  loadSprite (&tableSurface[BUTTON_RESET], pref.themePath, "button_reset.png");
  loadSprite (&tableSurface[BUTTON_BACKWARDS], pref.themePath, "button_backwards.png");
  loadSprite (&tableSurface[BUTTON_FILE], pref.themePath, "button_file.png");
  loadSprite (&tableSurface[BUTTON_SETTINGS], pref.themePath, "button_settings.png");
  loadSprite (&tableSurface[BUTTON_SHORTCUTS], pref.themePath, "button_shortcuts.png");
  loadSprite (&tableSurface[BUTTON_LEVELS], pref.themePath, "button_levels.png");
  loadSprite (&tableSurface[BUTTON_ABOUT], pref.themePath, "button_about.png");
  loadSprite (&tableSurface[BUTTON_HIGHLIGHT], pref.themePath, "button_highlight.png");


  /* outside the wall */
  loadSprite (&tableSurface[OUTSIDE_IMAGE], pref.themePath, "outside.png");
  loadSprite (&tableSurface[OUTSIDE2_IMAGE], pref.themePath, "outside2.png");
  loadSprite (&tableSurface[OUTSIDE3_IMAGE], pref.themePath, "outside3.png");
  loadSprite (&tableSurface[OUTSIDE4_IMAGE], pref.themePath, "outside4.png");

  /* Congrats */
  loadSprite (&tableSurface[CONGRATS], pref.themePath, "congrats_background.png");
  return EXIT_SUCCESS;
}


/* load a sprite as a surface */
void
loadSprite (Sprites * sprites, const char *themePath, const char *imageName)
{
  char filePath[MAX_CARACT] = "";
  strcpy (filePath, themePath);
  strcat (filePath, imageName);
  /* test if file exist */
  struct stat file_stat;
  if (stat (filePath, &file_stat) < 0)
    {
      printf (gettext ("the file %s does not exist\n"), filePath);
      strcpy (filePath, "img/original/");
      strcat (filePath, imageName);
      fprintf (stderr, gettext ("Load default image instead: %s\n\n"),
	       filePath);
    }
  else
    {
      printf (gettext ("%s exist\n"), filePath);
    }

  sprites->image = IMG_Load (filePath);
  if (sprites->image == NULL)
    {
      fprintf (stderr, gettext ("IMG_Load %s failed: %s\n"), filePath,
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
  fprintf (stderr, gettext ("\tAll surfaces are free %s\n"), SDL_GetError ());
}


char *
findFont (void)
{
  char *ret = malloc (MAX_CARACT);
  char fontPath[MAX_CARACT] =
    "/usr/share/fonts/truetype/roboto/hinted/Roboto-Bold.ttf";
  struct stat file_stat;
  if (stat (fontPath, &file_stat) < 0)
    {
      strcpy (fontPath, "/usr/share/fonts/truetype/roboto/Roboto-Bold.ttf");
      if (stat (fontPath, &file_stat) < 0)
	{
	  fprintf (stderr,
		   gettext
		   ("Roboto-Bold.ttf was not found. Install the fonts-roboto package\n"));
	  exit (EXIT_FAILURE);
	}
    }
  strcpy (ret, fontPath);
  return ret;
  free (ret);
}
#endif
