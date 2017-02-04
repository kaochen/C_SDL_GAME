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

#include <libintl.h>
#include <locale.h>

#include <sys/stat.h>

/*init the textSurface table */
void
tableTextSurface_init (S_Text tableTextSurface[NBR_OF_TAB][NBR_OF_TAB_LINE])
{
  /* Init all the table */
  for (size_t i = 0; i < NBR_OF_TAB; ++i){
    for (size_t j = 0; j < NBR_OF_TAB_LINE; ++j){
      tableTextSurface[i][j] = (S_Text){ .image = NULL,  .lineNbr = 1};
  }
}
  /* Init main Menu */
  size_t fontSize = 20, R = 255, G = 255, B = 255, A = 255;
  loadTextAsSurface (INFO,0, tableTextSurface, gettext ("Current Level Infos:"), fontSize, R,
		     G, B, A);
  loadTextAsSurface (SHORTCUTS,0, tableTextSurface, gettext ("Shortcuts:"), fontSize, R,
		     G, B, A);
  loadTextAsSurface (ABOUT,0, tableTextSurface, gettext ("About:"), fontSize, R,
		     G, B, A);
  loadTextAsSurface (FILES,0, tableTextSurface, gettext ("Files:"), fontSize, R,
		     G, B, A);
  loadTextAsSurface (SETTINGS,0, tableTextSurface, gettext ("Settings:"), fontSize, R,
		     G, B, A);

  /* Init sub Menu */
  /*shortcuts */
  size_t fontSizeSub = 20;
  loadTextAsSurface (SHORTCUTS,1, tableTextSurface,
		     gettext ("m : open and close menu"), fontSizeSub, R, G,
		     B, A);
  loadTextAsSurface (SHORTCUTS,2, tableTextSurface, gettext ("n : next level"),
		     fontSizeSub, R, G, B, A);
  loadTextAsSurface (SHORTCUTS,3, tableTextSurface,
		     gettext ("p : previous level"), fontSizeSub, R, G, B, A);
  loadTextAsSurface (SHORTCUTS,4, tableTextSurface,
		     gettext ("r : reset current level"), fontSizeSub, R, G,
		     B, A);
  loadTextAsSurface (SHORTCUTS,5, tableTextSurface, gettext ("q : quit game"),
		     fontSizeSub, R, G, B, A);
  /* Init sub Menu */
  /*about */
  loadTextAsSurface (ABOUT,1, tableTextSurface, gettext ("Website:"),
		     fontSizeSub, R, G, B, A);
  loadTextAsSurface (ABOUT,2, tableTextSurface,
		     gettext ("https://github.com/kaochen/SokoRobot"),
		     18, R, G, B, A);

  /* Settings */
  char text[MAX_CARACT] ="";
  sprintf(text,gettext("Window: %dx%dpx %dhz"),
                     pref.window_width, pref.window_height, pref.framerate);
  loadTextAsSurface (SETTINGS,1, tableTextSurface,text,fontSizeSub, R, G, B, A);

  sprintf(text,gettext("Theme: %s"), pref.themePath);
  loadTextAsSurface (SETTINGS,2, tableTextSurface,text,fontSizeSub, R, G, B, A);

}

/* free all text surfaces */
void
freeS_Text (S_Text tableTextSurface[NBR_OF_TAB][NBR_OF_TAB_LINE])
{

  for (size_t i = 0; i < NBR_OF_TAB; i++){
  for (size_t j = 0; j < NBR_OF_TAB_LINE; j++){
      if (tableTextSurface[i][j].image != NULL)
	      {
	        SDL_FreeSurface (tableTextSurface[i][j].image);
	      }
    }
    }
  fprintf (stderr, gettext ("\tAll text surfaces are free %s\n"),
	   SDL_GetError ());
}

/*make from a text a table and load it into the tableTextSurface*/
void
loadTextAsSurface (size_t tab, size_t lineNbr, S_Text tableTextSurface[NBR_OF_TAB][NBR_OF_TAB_LINE], char *text,
		   size_t fontSize, size_t R, size_t G, size_t B, size_t A)
{
  /* setup font */
  TTF_Font *font = NULL;
  char fontPath[MAX_CARACT] = "";
  strcpy (fontPath, findFont ());
  font = TTF_OpenFont (fontPath, fontSize);


  SDL_Color fontColor = { R, G, B, A };
  tableTextSurface[tab][lineNbr].image =  TTF_RenderUTF8_Blended (font, text, fontColor);
  //clean
  TTF_CloseFont (font);
  font = NULL;
}



/* display the level number */
int
levelMenu (SDL_Surface * screen,
           S_LevelList * levelList)
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
  sprintf (text, gettext ("Level: %d/%d"), pref.level, levelMax - 1);
  levelText = TTF_RenderUTF8_Blended (font, text, fontColor);

  /* blit the text */
  SDL_Rect pos;
  pos.x = (pref.x_menu + ((MENU_WIDTH - levelText->w)/ 2));
  pos.y = (SPRITE_SIZE - levelText->h) / 2;
  SDL_BlitSurface (levelText, NULL, screen, &pos);

  /* clean */
  SDL_FreeSurface (levelText);
  TTF_CloseFont (font);
  font = NULL;
  return EXIT_SUCCESS;
}


/* Get the first five name files */
int
loadFileName( S_Text tableTextSurface[NBR_OF_TAB][NBR_OF_TAB_LINE],
              S_FilesList *filesList)
{
  S_Files *actual = filesList->first;
  if (filesList == NULL || actual == NULL)
    {
      return (EXIT_FAILURE);
    }

 size_t fontSize = 20, R = 255, G = 255, B = 255, A = 255;
 char name[MAX_CARACT] = "";

 for (size_t i = 0; i < MENU_MAX_FILE; i++)
    {
     sprintf (name, "\t\t\t\t%s",extractNameFile(actual->name));
     loadTextAsSurface (FILES,(i+1), tableTextSurface, name, fontSize, R, G, B, A);
     actual = actual->next;
    }
return (EXIT_SUCCESS);
}

/*get the file name from is path*/
char *
extractNameFile(char *path){
 char *name = NULL;
 char c = '/';
 name = strrchr (path, c);
 return name;
}
#endif
