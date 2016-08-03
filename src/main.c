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

#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>
#include <errno.h>

#include "../inc/const.h"
#include "../inc/game.h"
#include "../inc/level.h"
#include "../inc/menu.h"
#include "../inc/settings.h"
#include "../inc/sprites.h"
#include "../inc/slc.h"


int main(int argc, char *argv[])
{
    /*manage argument (not used for now)*/
    int i;

    for (i=1; i < argc; i++)
    {
        printf("Argument %d : %s \n", i, argv[i]);
    }

    /* reset errors*/
    errno = 0;

    /* start random processor just once */
    srand(time(NULL));

    /* Start and check if SDL start correctly */
    if (SDL_Init(SDL_INIT_VIDEO) == -1) {
	fprintf(stderr, "SDL initialization error: %s\n", SDL_GetError());
	exit(EXIT_FAILURE);
    }

    /* Start SDL TTF */
    if (TTF_Init() == -1) {
	fprintf(stderr, "TTF_Init initialization error: %s\n",
		TTF_GetError());
	exit(EXIT_FAILURE);
    }

    /*load preferences*/

    printf("Window size %dx%d, maxBlocks: %d\n",getWindow_width(), getWindow_height(), getMax_Blocks());

    /* Create the window game */
    SDL_Window *window = SDL_CreateWindow(GAME_NAME,
					  SDL_WINDOWPOS_UNDEFINED,
					  SDL_WINDOWPOS_UNDEFINED,
					  getWindow_width(),
					  getWindow_height(),
					  SDL_WINDOW_MOUSE_FOCUS);

    /* Set window icon */
    SDL_SetWindowIcon(window, IMG_Load("img/icon.png"));

    /* Create a surface from the window */
    SDL_Surface *screen = NULL;
    screen = SDL_GetWindowSurface(window);

    /* Check if window and surface are created */
    if (window == NULL || screen == NULL) {
	fprintf(stderr, "Creating the main window failed: %s\n",
		SDL_GetError());
	exit(EXIT_FAILURE);
    }
    fprintf(stderr, "Creating the main window succeed\n\n");

    /* load images into a table of struct */
    Sprites tableSurface[NBR_OF_IMAGES];
    loadAllSprites(tableSurface);

    /* create a grid with coordinates x,y to locate things */
    Square grid[getMax_Blocks()][getMax_Blocks()];


    char levelName[MAX_CARACT]="";


    /*List slc files from the levels/ folder */
    fprintf(stderr, "Searching files in the levels folder.\n");
    S_FilesList *filesList = initFilesList();
    if (listSlcLevelFiles(filesList) == EXIT_SUCCESS) {
	fprintf(stderr, "Level folder explored.\n");
    } else {
	perror("listSlcLevelFiles failed");
	exit(EXIT_FAILURE);
    }
    fprintf(stderr, "\n");

    /*Read files name from the filesList to check */
    if (readFilesList(filesList) == EXIT_SUCCESS) {
	fprintf(stderr, "Files list readed.\n");
    } else {
	perror("readFilesList failed");
	exit(EXIT_FAILURE);
    }
    fprintf(stderr, "\n");


    /*Read level from slc file */
    fprintf(stderr, "Get info from each files %s\n", SDL_GetError());
    S_LevelList *levelList = initLevelList();
    //readLevelList(levelList);
    fprintf(stderr, "Read the level list.\n");

    if (readLevelsAttributs(filesList, levelList) == EXIT_SUCCESS) {
	fprintf(stderr, "Attributs from files loaded.\n");
    } else {
	perror("Error when loading levels attributs from files. Perror");
	exit(EXIT_FAILURE);
    }
    fprintf(stderr, "\n");

    /* count all levels from all files */
    int max_Levels = getNbrOfLevels(levelList);

    /*Load first game */

    int levelChoice =  readLevelFromSetting(levelList);
    fprintf(stderr, "Loading first level.\n");
    if (loadSlcLevel(levelChoice, levelList, grid) == EXIT_SUCCESS) {
	fprintf(stderr, "Level loaded\n");
    } else {
	perror("Impossible to load the level. Perror");
    }


    /* Set player position */
    int xPlayer = 0;
    int yPlayer = 0;

    /* display the level using the grid */
    displayLevel(grid, screen, tableSurface);

    /* display the top bar  */
    displayTopBar(levelChoice, screen, tableSurface, levelList, grid);

    /* refresh the window */
    SDL_UpdateWindowSurface(window);

/* wait for quit event */
    const int FPS = 10;
    Uint32 currentTime = 0;
    Uint32 previousTime = 0;
    int carryOn = 1, x = 0, y = 0, menuOpened = 0, menuChoice = 0;
    bool freezeCommand = false;
    SDL_Event event;
    while (carryOn) {
            displayLevel(grid, screen, tableSurface);
	    //SDL_WaitEvent(&event);
        while(SDL_PollEvent(&event)){

	        switch (event.type) {
	            case SDL_QUIT:
	                carryOn = 0;
	                break;
	            case SDL_KEYDOWN:
	                /* Get the player position */
	                for (x = 0; x < getX_Blocks(); x++) {
		            for (y = 0; y < getY_Blocks(); y++) {
		                if (grid[x][y].mainRole == PLAYER){
			            xPlayer = x;
			            yPlayer = y;
                        fprintf(stderr, "player on %d:%d\n", xPlayer, yPlayer);
		                }
		            }
	                }
	                /* listen keyboard */
	                switch (event.key.keysym.sym) {
	                case SDLK_RIGHT:
		            /*Do not move when level is finished */
		            if (freezeCommand == true)
		                break;
		            /* Don't go outside */
		            if (xPlayer + 1 >= getX_Blocks())
		                break;
		            /* Test if wall */
		            if (grid[xPlayer + 1][yPlayer].mainRole == WALL)
		                break;
		            /* Don't go outside with a box */
		            if (grid[xPlayer + 1][yPlayer].mainRole == BOX
		                && xPlayer + 2 >= getX_Blocks())
		                break;
		            /* Do not move a box if it is close to a wall or an other box */
		            if (grid[xPlayer + 1][yPlayer].mainRole == BOX
		                && grid[xPlayer + 2][yPlayer].mainRole == BOX
		                && grid[xPlayer + 2][yPlayer].mainRole == WALL)
		                break;
		            /* Move a box only if there is space to do it */
		            if ((grid[xPlayer + 1][yPlayer].mainRole == BOX
		                && grid[xPlayer + 2][yPlayer].mainRole == GROUND)
		                || (grid[xPlayer + 1][yPlayer].mainRole == BOX
		                && grid[xPlayer + 2][yPlayer].goalRole == GOAL)) {

		                /* clean old position */
		                grid[xPlayer][yPlayer].mainRole = GROUND;
                      grid[xPlayer][yPlayer].playerRole = EMPTY;

    	                /* update new position */
		                grid[xPlayer + 2][yPlayer].mainRole = BOX;
		                grid[xPlayer + 1][yPlayer].mainRole = PLAYER;
                      grid[xPlayer + 1][yPlayer].playerRole = PLAYER_R;
		                break;
		            }
		            /* move only on grounds and Goals */
		            if (grid[xPlayer + 1][yPlayer].mainRole == GROUND
		                || grid[xPlayer + 1][yPlayer].goalRole == GOAL) {

                        /* clean old position */
		                  grid[xPlayer][yPlayer].mainRole = GROUND;
                        grid[xPlayer][yPlayer].playerRole = EMPTY;

    	                /* update new position */
		                grid[xPlayer + 1][yPlayer].mainRole = PLAYER;
                        grid[xPlayer + 1][yPlayer].playerRole = PLAYER_R;
		                break;
		            }
		            break;

	                case SDLK_LEFT:
		            /*Do not move when level is finished */
		            if (freezeCommand == true)
		                break;

		            /* Don't go outside */
		            if (xPlayer - 1 < 0)
		                break;
		            /* test if wall */
		            if (grid[xPlayer - 1][yPlayer].mainRole == WALL)
		                break;
		            /* Don't go outside with a box */
		            if (grid[xPlayer - 1][yPlayer].mainRole == BOX
		                && xPlayer - 2 < 0)
		                break;
		            /* Do not move a box if it is close to a wall or an other box */
		            if (grid[xPlayer - 1][yPlayer].mainRole == BOX
		                && grid[xPlayer - 2][yPlayer].mainRole == BOX
		                && grid[xPlayer - 2][yPlayer].mainRole == WALL)
		                break;
		            /* Move a box only if there is space to do it */
		            if ((grid[xPlayer - 1][yPlayer].mainRole == BOX
		                && grid[xPlayer - 2][yPlayer].mainRole == GROUND)
		                || (grid[xPlayer - 1][yPlayer].mainRole == BOX
		                && grid[xPlayer - 2][yPlayer].goalRole == GOAL)) {

                        /* clean old position */
		                grid[xPlayer][yPlayer].mainRole = GROUND;
                        grid[xPlayer][yPlayer].playerRole = EMPTY;

    	                /* update new position */
		                grid[xPlayer - 2][yPlayer].mainRole = BOX;
		                grid[xPlayer - 1][yPlayer].mainRole = PLAYER;
                        grid[xPlayer - 1][yPlayer].playerRole = PLAYER_L;
		                break;
		            }
		            /* move only on grounds and Goals */
		            if (grid[xPlayer - 1][yPlayer].mainRole == GROUND
		                || grid[xPlayer - 1][yPlayer].goalRole == GOAL) {

                        /* clean old position */
		                grid[xPlayer][yPlayer].mainRole = GROUND;
                        grid[xPlayer][yPlayer].playerRole = EMPTY;

                        /* update new position */
		                grid[xPlayer - 1][yPlayer].mainRole = PLAYER;
                        grid[xPlayer - 1][yPlayer].playerRole = PLAYER_L;
		                break;
		            }
		            break;

	                case SDLK_UP:
                    if (menuOpened == 0){
		                /*Do not move when level is finished */
		                if (freezeCommand == true)
		                    break;

		                /* Don't go outside */
		                if (yPlayer - 1 < 0)
		                    break;
		                /* test if wall */
		                if (grid[xPlayer][yPlayer - 1].mainRole == WALL)
		                    break;
		                /* Don't go outside with a box */
		                if (grid[xPlayer][yPlayer - 1].mainRole == BOX
		                    && yPlayer - 2 < 0)
		                    break;
		                /* Do not move a box if it is close to a wall or an other box */
		                if (grid[xPlayer][yPlayer - 1].mainRole == BOX
		                    && grid[xPlayer][yPlayer - 2].mainRole == BOX
		                    && grid[xPlayer][yPlayer - 2].mainRole == WALL)
		                    break;
		                /* Move a box only if there is space to do it */
		                if ((grid[xPlayer][yPlayer - 1].mainRole == BOX
		                    && grid[xPlayer][yPlayer - 2].mainRole == GROUND)
		                    || (grid[xPlayer][yPlayer - 1].mainRole == BOX
		                    && grid[xPlayer][yPlayer - 2].goalRole == GOAL)) {

                            /* clean old position */
		                    grid[xPlayer][yPlayer].mainRole = GROUND;
                            grid[xPlayer][yPlayer].playerRole = EMPTY;

                            /* update new position */
		                    grid[xPlayer][yPlayer - 2].mainRole = BOX;
		                    grid[xPlayer][yPlayer - 1].mainRole = PLAYER;
                            grid[xPlayer][yPlayer - 1].playerRole = PLAYER_B;
		                    break;
		                }
		                /* move only on grounds and Goals */
		                if (grid[xPlayer][yPlayer - 1].mainRole == GROUND
		                    || grid[xPlayer][yPlayer - 1].goalRole == GOAL) {

                            /* clean old position */
    		                grid[xPlayer][yPlayer].mainRole = GROUND;
                            grid[xPlayer][yPlayer].playerRole = EMPTY;

                            /* update new position */
		                    grid[xPlayer][yPlayer - 1].mainRole = PLAYER;
                            grid[xPlayer][yPlayer - 1].playerRole = PLAYER_B;
		                    break;
		                }
                    }
                    else if (menuOpened == 1)
                      {
                        menuChoice--;
                        /* make circle */
                        if (menuChoice == -1){
                          menuChoice = 2;
                        }
                        openMenu(screen,tableSurface, levelList,menuChoice,levelChoice);
                      }
		            break;

	                case SDLK_DOWN:
                    if (menuOpened == 0){
		                /*Do not move when level is finished */
		                if (freezeCommand == true)
		                    break;

		                /* Don't go outside */
		                if (yPlayer + 1 >= getY_Blocks())
		                    break;
		                /* test if wall */
		                if (grid[xPlayer][yPlayer + 1].mainRole == WALL)
		                    break;
		                /* Don't go outside with a case */
		                if (grid[xPlayer][yPlayer + 1].mainRole == BOX
		                    && yPlayer + 2 >= getY_Blocks())
		                    break;
		                /* Do not move a box if it is close to a wall or an other box */
		                if (grid[xPlayer][yPlayer + 1].mainRole == BOX
		                    && grid[xPlayer][yPlayer + 2].mainRole == BOX
		                    && grid[xPlayer][yPlayer + 2].mainRole == WALL)
		                    break;
		                /* Move a box only if there is space to do it */
		                if ((grid[xPlayer][yPlayer + 1].mainRole == BOX
		                    && grid[xPlayer][yPlayer + 2].mainRole == GROUND)
		                    || (grid[xPlayer][yPlayer + 1].mainRole == BOX
		                    && grid[xPlayer][yPlayer + 2].goalRole == GOAL)) {

                            /* clean old position */
		                    grid[xPlayer][yPlayer].mainRole = GROUND;
                            grid[xPlayer][yPlayer].playerRole = EMPTY;

                            /* update new position */
		                    grid[xPlayer][yPlayer + 2].mainRole = BOX;
		                    grid[xPlayer][yPlayer + 1].mainRole = PLAYER;
                            grid[xPlayer][yPlayer + 1].playerRole = PLAYER_F;
		                    break;
		                }
		                /* move only on grounds and Goals */
		                if (grid[xPlayer][yPlayer + 1].mainRole == GROUND
		                    || grid[xPlayer][yPlayer + 1].goalRole == GOAL) {

                            /* clean old position */
		                    grid[xPlayer][yPlayer].mainRole = GROUND;
                            grid[xPlayer][yPlayer].playerRole = EMPTY;

                            /* update new position */
		                    grid[xPlayer][yPlayer + 1].mainRole = PLAYER;
                            grid[xPlayer][yPlayer + 1].playerRole = PLAYER_F;
		                    break;
		                }
                    }
                    else if (menuOpened == 1)
                      {
                        menuChoice++;
                        /* make circle */
                        if (menuChoice == 3){
                          menuChoice = 0;
                        }
                        openMenu(screen,tableSurface,levelList, menuChoice, levelChoice);
                      }
		                break;

		                /* hit m to open or close the menu */

	                    case SDLK_m:
                       if (menuOpened == 0){
                       fprintf(stderr, "opening Menu\n");
                         freezeCommand = true;
                         menuOpened = 1;
                       }
                       else{
                         fprintf(stderr, "closing Menu\n");
                         /*reset status */
                         freezeCommand = false;
                         menuOpened = 0;
                         menuChoice = 0;
                       }
                        break;


		            /* hit r to reset the current level */
	                case SDLK_r:
		            /* load the level */
		            if (loadSlcLevel(levelChoice, levelList, grid) ==
		                EXIT_SUCCESS) {
		                fprintf(stderr, "Level loaded\n");
		            } else {
		                perror("Impossible to load the level. Perror");
		            }

		            /* display the level using grid */
		            displayLevel(grid, screen, tableSurface);

		            /*reset status */
                    freezeCommand = false;
                    menuOpened = 0;
		            fprintf(stderr, "Level %d\n loaded", (levelChoice + 1));
		            break;

		            /* hit n to load the next level */
	                case SDLK_n:
		            /* load next the level */
		            levelChoice += 1;
		            if (levelChoice == max_Levels)
		                levelChoice = 0;
		            if (loadSlcLevel(levelChoice, levelList, grid) ==
		                EXIT_SUCCESS) {
		                fprintf(stderr, "Level loaded\n");
		            } else {
		                perror("Impossible to load the level. Perror");
		            }
		            /* display the level using grid */
		            displayLevel(grid, screen, tableSurface);

		            /*reset status */
                    freezeCommand = false;
                    menuOpened = 0;
		            fprintf(stderr, "Level %d\n loaded", (levelChoice));
		            break;

		            /* hit p to load the previous level */
	                case SDLK_p:
		            /* load previous level */
		            levelChoice -= 1;
		            if (levelChoice == -1)
		                levelChoice = max_Levels - 1;

		            if (loadSlcLevel(levelChoice, levelList, grid) ==
		                EXIT_SUCCESS) {
		                fprintf(stderr, "Level loaded\n");
		            } else {
		                perror("Impossible to load the level. Perror");
		            }

		            /* display the level using grid */
		            displayLevel(grid, screen, tableSurface);

		            /*reset status */
		            freezeCommand = false;
                  menuOpened = 0;
		            fprintf(stderr, "Level %d\n loaded", (levelChoice));

		            break;
		            /* hit q to quit */
	                case SDLK_q:
                  /* write last level name before closing */
                     strcpy(levelName, "");
                      getLevelName(levelChoice,levelList,levelName);
                      writePrefChar("LevelName", levelName);
		            carryOn = 0;
                    fprintf(stderr, "\nThe quit command (q) has been pressed.\n");
                    fprintf(stderr, "Start ending programm:\n");
		            break;

                    } //end of  switch (event.type)
            } //end of switch (event.type) {


       /*display congrats if the level is finished*/
	    if (levelFinished(grid, screen, tableSurface) == FINISH) {
		    freezeCommand = true;}
            }  // end of while(SDL_PollEvent(&event)){

      if (menuOpened == 1){
       openMenu(screen, tableSurface, levelList, menuChoice, levelChoice);
      }

	   displayTopBar((levelChoice), screen, tableSurface,levelList,grid);

	    SDL_UpdateWindowSurface(window);
        /* setup delay using FPS */
        currentTime = SDL_GetTicks();
        if((currentTime-previousTime) < (unsigned)(1000/FPS) ) {
                SDL_Delay((unsigned)(1000/FPS)-(currentTime-previousTime));

	    }
        else{previousTime = currentTime;
        }

        } // end of   while (carryOn){


    /* clean */
    fprintf(stderr,"- Destroying level list\n");
    destroy(levelList);
    free(levelList);
    fprintf(stderr,"- Destroying file list\n");
    destroyFileList(filesList);
    free(filesList);
    //fprintf(stderr,"-Freeing Sprites\n");
    //freeSprites(tableSurface);
    SDL_FreeSurface(screen);
    fprintf(stderr,"- Free SDL screen\n");
    SDL_DestroyWindow(window);
    fprintf(stderr,"- Free SDL window\n");
    TTF_Quit();
    fprintf(stderr,"- TFF is out\n");
    SDL_Quit();
    fprintf(stderr,"- SDL is out\n");
    fprintf(stderr, "Everythings seems good, Bye\n");
    return EXIT_SUCCESS;
}
