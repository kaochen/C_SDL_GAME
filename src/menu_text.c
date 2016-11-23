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

#include "../inc/menu.h"
#include "../inc/menu_text.h"

#include <libintl.h>
#include <locale.h>

#include <sys/stat.h>

/*init the textSurface table */
void
tableTextSurface_init (S_Text tableTextSurface[NBR_OF_TEXT])
{
  /* Init all the table */
  for (size_t i = 0; i < NBR_OF_TEXT; ++i)
    tableTextSurface[i] = (S_Text)
    {
    .textSurface = NULL};

  /* Init main Menu */
  size_t fontSize = 18, R = 255, G = 255, B = 255, A = 255;
  loadTextAsSurface (INFO, tableTextSurface, gettext ("Current Level Infos"), fontSize, R,
		     G, B, A);
  loadTextAsSurface (SHORTCUTS, tableTextSurface, gettext ("Shortcuts"), fontSize, R,
		     G, B, A);
  loadTextAsSurface (ABOUT, tableTextSurface, gettext ("About"), fontSize, R,
		     G, B, A);

  /* Init sub Menu */
  /*shortcuts */
  size_t fontSizeSub = 18;
  loadTextAsSurface (SHORTCUTS1, tableTextSurface,
		     gettext ("m : open and close menu"), fontSizeSub, R, G,
		     B, A);
  loadTextAsSurface (SHORTCUTS2, tableTextSurface, gettext ("n : next level"),
		     fontSizeSub, R, G, B, A);
  loadTextAsSurface (SHORTCUTS3, tableTextSurface,
		     gettext ("p : previous level"), fontSizeSub, R, G, B, A);
  loadTextAsSurface (SHORTCUTS4, tableTextSurface,
		     gettext ("r : reset current level"), fontSizeSub, R, G,
		     B, A);
  loadTextAsSurface (SHORTCUTS5, tableTextSurface, gettext ("q : quit game"),
		     fontSizeSub, R, G, B, A);
  /* Init sub Menu */
  /*about */
  loadTextAsSurface (ABOUT1, tableTextSurface, gettext ("Website :"),
		     fontSizeSub, R, G, B, A);
  loadTextAsSurface (ABOUT2, tableTextSurface,
		     gettext ("https://github.com/kaochen/SokoRobot"),
		     fontSizeSub, R, G, B, A);
}

/* free all text surfaces */
void
freeS_Text (S_Text tableTextSurface[NBR_OF_TEXT])
{

  for (size_t i = 0; i < NBR_OF_TEXT; i++)
    {
      if (tableTextSurface[i].textSurface != NULL)
	{
	  SDL_FreeSurface (tableTextSurface[i].textSurface);
	}
    }
  fprintf (stderr, gettext ("\tAll text surfaces are free %s\n"),
	   SDL_GetError ());
}

/*make from a text a table and load it into the tableTextSurface*/
void
loadTextAsSurface (size_t t, S_Text tableTextSurface[NBR_OF_TEXT], char *text,
		   size_t fontSize, size_t R, size_t G, size_t B, size_t A)
{
  /* setup font */
  TTF_Font *font = NULL;
  char fontPath[MAX_CARACT] = "";
  strcpy (fontPath, findFont ());
  font = TTF_OpenFont (fontPath, fontSize);


  SDL_Color fontColor = { R, G, B, A };
  tableTextSurface[t].textSurface =
    TTF_RenderUTF8_Blended (font, text, fontColor);
  //clean
  TTF_CloseFont (font);
  font = NULL;
}



/* display the level number */
int
levelMenu (int levelNumber, SDL_Surface * screen, S_LevelList * levelList)
{
  if (screen == NULL || levelList == NULL)
    {
      fprintf (stderr, gettext ("init levelMenu failed: %s\n"),
	       SDL_GetError ());
      return EXIT_FAILURE;
    }

  /* setup font */
  TTF_Font *font = NULL;
  char fontPath[MAX_CARACT] = "";
  strcpy (fontPath, findFont ());

  font = TTF_OpenFont (fontPath, 20);

  SDL_Color fontColor = { 255, 255, 255, 255 };

  /* count all levels from all files */
  int levelMax = getNbrOfLevels (levelList);

  /* indicate level nbr */
  SDL_Surface *levelText = NULL;
  char text[20] = "";
  sprintf (text, gettext ("Level: %d/%d"), levelNumber, levelMax - 1);
  levelText = TTF_RenderUTF8_Blended (font, text, fontColor);

  /* blit the text */
  SDL_Rect pos;
  pos.x = (menuPosX () + ((MENU_WIDTH - levelText->w)/ 2));
  pos.y = (SPRITE_SIZE - levelText->h) / 2;
  SDL_BlitSurface (levelText, NULL, screen, &pos);

  /* clean */
  SDL_FreeSurface (levelText);
  TTF_CloseFont (font);
  font = NULL;
  return EXIT_SUCCESS;
}

#endif
