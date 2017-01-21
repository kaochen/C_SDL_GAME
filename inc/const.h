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
#define MENU_MAX_INFO 3
#define MENU_MAX_FILE 8
#define MENU_MAX_SHORTCUTS 5
#define MENU_MAX_SETTINGS 2
#define MENU_MAX_ABOUT 2

#define NBR_OF_TAB 6
#define NBR_OF_TAB_LINE 10


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
	MENU_CIRCLE,MENU_CURVE,MENU_H_LINE,MENU_V_LINE,
  BUTTON_ARROW_P,BUTTON_ARROW_L,BUTTON_RESET,BUTTON_BACKWARDS,
	BUTTON_FILE,BUTTON_SETTINGS,BUTTON_SHORTCUTS,BUTTON_LEVELS,BUTTON_ABOUT,
	BUTTON_HIGHLIGHT,
  OUTSIDE_IMAGE, OUTSIDE2_IMAGE, OUTSIDE3_IMAGE, OUTSIDE4_IMAGE,
  CONGRATS};

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
 int tab;
 int menu;
 int image;
}S_Menu;

typedef struct
{
  int name;
  int nbrLines;
  int image;
}S_Tabs;

/*Struct to store menu status*/
typedef struct
{
 S_Tabs tab[NBR_OF_TAB];
 int tabChoice;
 int lineChoice;
 int nbrTabs;
 int open; //the menu panel is open or not
 int freeze; //Freeze the gameplay when menu or congrats are open
 int xPos;
 int yPos;
}S_menuchoice;

S_menuchoice menuChoice; //store current menu status as a  global struct

/*Struct to store settings*/
typedef struct
{
 /* Main Window */
 int display_width;
 int display_height;
 int window_height;
 int window_width;
 int x_Blocks; //number of SPRITE_SIZE in  window_width
 int y_Blocks; //number of SPRITE_SIZE in  window_height
 int max_X_Blocks;
 int max_Y_Blocks;

 /*Menu */
 int x_menu; // menu position on x
 int xb_menu; // menu position on x in blocks
 int y_menu; // menu position on y
 int h_menu_block; // menu height in block;


 /* Level */
 int level;
 int level_max;
 int reload;

 /* framerate */
 int display_framerate;
 int framerate;

 /* theme */
  char themePath[MAX_CARACT];

}S_preferences;
S_preferences pref; //store all game settings and preferences

#endif
