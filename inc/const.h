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

#ifndef CONST_H
#define CONST_H

#define GAME_NAME "SokoRobot"

#define PREF_FILE "preferences.ini"

//Define Window size
#define W_WIDTH  400
#define W_HEIGHT 400

//define size of a sprite in px
#define SPRITE_SIZE 40

#define MENU_WIDTH 400
#define MENU_LINE 15
#define MENU_ROW 15
#define MENU_MAX_INFO 2
#define MENU_MAX_SHORTCUTS 4
#define MENU_MAX_ABOUT 1
#define MENU_MAX_FILE 1


//autorise 100 images max
#define NBR_OF_IMAGES 50
#define NBR_OF_TEXT 20

#define MAX_CARACT 200

#define PI 3.14159265

/* define role for each square*/
typedef struct
{
  int mainRole;			//GROUND, OUTSIDE, WALL, BOX
  int subRole;			// OUTSIDE, OUTSIDE2, OUTSIDE3, OUTSIDE4, WALL1, WALL2, WALL3, WALL4,
  int goalRole;			// GOAL, BOX, BOX_OK, EMPTY,
  int boxRole;			//BOX, BOX_OK, EMPTY
  int playerRole;		//PLAYER_F,PLAYER_B,PLAYER_L,PLAYER_R, EMPTY,
  int target; 			//TARGET,EMPTY
} Square;

typedef enum bool bool;

enum bool
{
  true = 1, false = 0
};

//object type
enum
{ GROUND, GROUND1, GROUND2, GROUND3, GROUND4, OUTSIDE, OUTSIDE2,
  OUTSIDE3, OUTSIDE4, WALL, WALL1, WALL2, WALL3, WALL4, GOAL, BOX, BOX_OK,
  TARGET,
  PLAYER, EMPTY,
  PLAYER_F, PLAYER_B, PLAYER_L, PLAYER_R
};

//direction
enum
{ UP, DOWN, RIGHT, LEFT, STILL };
enum
{ RANDOM, FIXED };

//image names
enum
{ NO_IMAGE,
  PLAYER_FRONT, PLAYER_BACK, PLAYER_LEFT, PLAYER_RIGHT,
  TARGET_IMAGE,
  WALL1_IMAGE, WALL2_IMAGE, WALL3_IMAGE, WALL4_IMAGE, WALL_TOP,
  WALL_BOTTOM, WALL_LEFT, WALL_RIGHT,
  CORNER_TR, CORNER_TL, CORNER_BR, CORNER_BL,
  GROUND1_IMAGE, GROUND2_IMAGE, GROUND3_IMAGE,
  BOX_IMAGE, BOX_IMAGE_OK,
  GOAL_IMAGE,
  MENU_BAR, MENU_RIBBON, MENU_BOTTOM, MENU_TOP_LIGHT, PROGRESS, MENU_SQUARE,
	MENU_SEPARATOR,MENU_OVERTEXT,MENU_HIGHLIGHT,
  BUTTON_ARROW_P,BUTTON_ARROW_L,BUTTON_RESET,BUTTON_BACKWARDS,BUTTON_FILE,
  OUTSIDE_IMAGE, OUTSIDE2_IMAGE, OUTSIDE3_IMAGE, OUTSIDE4_IMAGE,
  CONGRATS
};
enum
{ FINISH, NOT_FINISHED };

/* Structures ============================================ */

/* Files ========================================= */
/* files definion struct */
typedef struct S_Files S_Files;
struct S_Files
{
  char *name;
  S_Files *next;
  S_Files *previous;
};

/* control files list */
typedef struct S_FilesList S_FilesList;
struct S_FilesList
{
  S_Files *first;
  S_Files *last;
  int nbr_of_files;
};

/* Levels ========================================= */

/* level definion struct */
typedef struct S_Level S_Level;
struct S_Level
{
  char *name;
  char *fileName;
  int height;
  int width;
  S_Level *next;
  S_Level *previous;
};

/* control level list */
typedef struct {
  S_Level *first;
  S_Level *last;
  int nbr_of_levels;
} S_LevelList;

/* Menus ========================================= */

typedef struct
{
 int role;
 int type;
 int text;
 int menu;
 int x;
 int y;
}S_Menu;

/*Struct to store menu status*/
typedef struct
{
 int tab;
 int sub; //under tabs
 int max; //max lines in sub part
 int open; //the menu panel is open or not
 int freeze; //Freeze the gameplay when menu or congrats are open
}S_menuchoice;

/*Struct to store settings*/
typedef struct
{
 int window_height;
 int window_width;
 int framerate;
}S_preferences;

#endif
