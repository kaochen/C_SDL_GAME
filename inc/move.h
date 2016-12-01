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

#ifndef MOVE_H
#define MOVE_H

#include <stdlib.h>
#include <stdio.h>

#include "const.h"
#include "settings.h"


/* move player and update object status" */
int
movePlayer (S_preferences *pref,
            int xPlayer,
            int yPlayer,
            int direction ,
            Square grid[pref->max_X_Blocks][pref->max_Y_Blocks]);

/*Find the player on the grid*/
void
getPosPlayer(S_preferences *pref,
             int *xPlayer,
             int *yPlayer,
             Square grid[pref->max_X_Blocks][pref->max_Y_Blocks]);

int
mouseMoveDirection(int xPlayer, int yPlayer, int xCursor, int yCursor);

/*choose where the target image will be displayed*/
void
moveTarget(S_preferences *pref,
           int next_target,
           int xPlayer,
           int yPlayer,
           Square grid[pref->max_X_Blocks][pref->max_Y_Blocks]);
#endif
