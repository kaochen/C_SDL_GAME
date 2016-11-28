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

#include "../inc/mouse.h"
/*detect mouse motion*/
int
mouseMotion(S_menuchoice *menuChoice, int xCursor, int yCursor, S_Menu gridMenu[MENU_LINE]){
    int start = menuPosX();
    int xGrid = (xCursor-start)/SPRITE_SIZE;
    int yGrid = yCursor/SPRITE_SIZE;
    int ySub = yCursor/(SPRITE_SIZE-10);
    if (menuChoice->open == 1){
    /* tabs */
    for (size_t i = 0; i < MENU_LINE; i++){   /*tabs or equal to 2 SPRITE_SIZE*/
        int  x = xGrid;
        //fprintf (stderr, "for %d:%d ", xGrid, yGrid);
        /*use size of the buttons*/
        if (gridMenu[i].type == OPENMENU || xGrid % 2 == 0)
            x--;

        //fprintf (stderr, "use %d:%d\n", x, yGrid);
        if ((gridMenu[i].x == xGrid || gridMenu[i].x == x) && gridMenu[i].y == yGrid && gridMenu[i].type == OPENMENU){
			    menuChoice->tab = gridMenu[i].menu;
                //fprintf (stderr, "match for %d:%d\n", x, yGrid);
                return 1; //refresh
                }
            }
      /* Lines in the sub menu */
    if(xGrid > 0 && xGrid < (MENU_WIDTH/SPRITE_SIZE) && ySub > 3 && ySub <= 4 + menuChoice->max ){
        menuChoice->sub = ySub - 4;
        //fprintf (stderr, "menuChoice : %d:%d\n", menuChoice->sub, ySub);
        return 1; //refresh
        }
    }
    return 0; //no refresh
}

#endif
