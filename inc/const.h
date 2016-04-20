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

//Define Window size
#define W_WIDTH  720
#define W_HEIGHT 720

//define size of a square in px
#define BOX_SIZE 40

//number of blocks
#define MAX_BLOCKS 18 //Must be higther than X_BLOCKS or Y_BLOCKS
#define X_BLOCKS 18
#define Y_BLOCKS 18

//autorise 100 images max
#define NBR_OF_IMAGES 100

#define MAX_CARACT 200

/* define role for each square*/
typedef struct
{
   int roleType;
   int groundType;
} Square;



//object type
enum {GROUND, GROUND1, GROUND2, GROUND3, GROUND4, WALL , GOAL,BOX, BOX_OK, PLAYER  };

//direction
enum {UP , DOWN, RIGHT, LEFT, STILL };
enum {RANDOM, FIXED};

//image names
enum {PLAYER_FRONT,PLAYER_BACK,PLAYER_LEFT,PLAYER_RIGHT,
  WALL1_IMAGE,WALL2_IMAGE,WALL3_IMAGE,WALL4_IMAGE, WALL_TOP, WALL_BOTTOM, WALL_LEFT, WALL_RIGHT,
  GROUND1_IMAGE,GROUND2_IMAGE,GROUND3_IMAGE,
  BOX_IMAGE, BOX_IMAGE_OK,
  GOAL_IMAGE,
  MENU_BACK,
};
enum {FINISH,NOT_FINISHED};
#endif
