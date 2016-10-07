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

/*This file handles texts of the menu.*/

#ifndef MENU_TEXT_C
#define MENU_TEXT_C

#include "../inc/menu_text.h"

#include <libintl.h>
#include <locale.h>

#include <sys/stat.h>

/* display shortcut in the menu */
int
displayShortcut (SDL_Surface * screen)
{

    if ( screen == NULL)
    {
      fprintf (stderr, gettext("init displayShortcut failed: %s\n"),
	       SDL_GetError ());
      return EXIT_FAILURE;
    }
  /* setup font */
  TTF_Font *font = NULL;
  char fontPath[MAX_CARACT] = "";
  strcpy(fontPath,findFont());

  font = TTF_OpenFont (fontPath, 20);
  SDL_Color fontColor = { 255, 255, 255, 255 };
  SDL_Surface *shortCutText = NULL;
  shortCutText = TTF_RenderText_Blended (font, "(M)", fontColor);
  /* blit the text */
  SDL_Rect shortCutTextPos;
  shortCutTextPos.x = menuPosX () + (MENU_WIDTH - 2 * SPRITE_SIZE - 5);
  shortCutTextPos.y = (SPRITE_SIZE - shortCutText->h)/2;
  SDL_BlitSurface (shortCutText, NULL, screen, &shortCutTextPos);

  /* clean */
  SDL_FreeSurface (shortCutText);
  TTF_CloseFont (font);
   font = NULL;
  return EXIT_SUCCESS;
}

/* display the level number */
int
levelMenu (int levelNumber, SDL_Surface * screen, S_LevelList * levelList)
{

   if ( screen == NULL || levelList == NULL)
    {
      fprintf (stderr, gettext("init levelMenu failed: %s\n"),
	       SDL_GetError ());
      return EXIT_FAILURE;
    }

  /* setup font */
  TTF_Font *font = NULL;
  char fontPath[MAX_CARACT] = "";
  strcpy(fontPath,findFont());

  font = TTF_OpenFont (fontPath, 20);

  SDL_Color fontColor = { 255, 255, 255, 255 };

  /* count all levels from all files */
  int levelMax = getNbrOfLevels (levelList);

  /* indicate level nbr */
  SDL_Surface *levelText = NULL;
  char text[20] = "";
  sprintf (text, gettext("Level: %d/%d"), levelNumber, levelMax-1);
  levelText = TTF_RenderUTF8_Blended (font, text, fontColor);

  /* blit the text */
  SDL_Rect levelTextPos;
  levelTextPos.x = (getWindow_width ()  - levelText->w)/2;
  levelTextPos.y = (SPRITE_SIZE - levelText->h)/2;
  SDL_BlitSurface (levelText, NULL, screen, &levelTextPos);

  /* clean */
  SDL_FreeSurface (levelText);
  TTF_CloseFont (font);
  font = NULL;
  return EXIT_SUCCESS;
}

#endif
