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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <SDL2/SDL.h>
#include "const.h"

/* Setup all paths */
void
setupPath(char * argv0);

//Try to find a user place to store SESSION_FILE
void
setupSessionFilePath();

/* load settings from pref file */
int
loadPrefStruct(SDL_DisplayMode current);

/*get int value from a ini file */
int
readInt (const char *fileName, const char *prefName);

/*get char value from the preference file */
void readChar(const char *fileName, char * ret, const char *setting);

//look at the session file if the current level has not been already achieved
int
testIfLevelAchieved(const char *levelName,
                    S_Menu gridMenu[pref.max_X_Blocks][pref.max_Y_Blocks]);

int
getWindow_width (void);

int
getWindow_height (void);

int
getWindow_framerate (void);


/* write a pref char */
int
writeChar (const char *fileName ,const char *lineName, const char *value);

/*get the theme Path*/
int
getThemePath(void);

// print only if verbose mode is on.
void
vbPrintf(const char *format, ...);

//store the names of the levels already achieved
int
storeLevelsFinished(const char *fileName, const char *levelName);

#endif
