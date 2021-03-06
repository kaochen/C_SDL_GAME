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

/*All mouse functions.*/

#ifndef MOUSE_C
#define MOUSE_C

#include "mouse.h"

/*detect mouse motion*/
int
mouseMotion(int xCursor,
            int yCursor,
            S_Menu gridMenu[pref.max_X_Blocks][pref.max_Y_Blocks]){

    int x = xCursor/SPRITE_SIZE;
    int y = yCursor/SPRITE_SIZE;
          // fprintf (stderr, "menuChoice x:%d/y:%d\n", x,y);
      if (menuChoice.open == 1){
        if (gridMenu[x][y].type == TABS){
           menuChoice.xPos = x;
           menuChoice.yPos = 1;
          if (menuChoice.tabChoice != gridMenu[x][y].tab)
            {
			        menuChoice.lineChoice = MENU_OFFSET;
              menuChoice.tabChoice = gridMenu[x][y].tab;
            }
          gridMenu_initLines(gridMenu);
        }

        if (gridMenu[x][y].type == TABS_LINE){
            menuChoice.lineChoice = y;
           //fprintf (stderr, "menuChoice x:%d/y:%d\n", x,y);
            }
           return 1; //refresh
      }

    return 0; //no refresh
}
#endif
