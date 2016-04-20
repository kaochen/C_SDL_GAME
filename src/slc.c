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

#ifndef SLC_C
#define SLC_C



#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <string.h>

#include "../inc/slc.h"
#include "../inc/const.h"

void openSlcFile ()
{
   char nameFile[MAX_CARACT] = "levels/12_Blocks.slc";
   char line[MAX_CARACT] = "";
   long cursor1 = 0, cursor2 = 0;
   FILE *slcFile = NULL;
   slcFile = fopen (nameFile, "r");

   /* check if file exist */
   if (slcFile != NULL)
   {
        fprintf (stderr, "%s opened %s\n", nameFile, SDL_GetError ());
   }
  else
    {
          fprintf (stderr, "Can't open %s : %s\n", nameFile, SDL_GetError ());
    }

	while (fgets(line, MAX_CARACT, slcFile)!= NULL)
	    {
	  	if(strstr(line, "<Description") != NULL)
		{
		     	cursor1 = ftell(slcFile);
		  	fprintf(stderr, "%s", line );
		}
	      	if(strstr(line, "</Description>") != NULL)
		{
		     	cursor2 = ftell(slcFile);
		  	fprintf(stderr, "%s", line );
		}

	    }

  	fseek(slcFile, cursor1, SEEK_SET);
	int i = 0, c = 0, l = 0;
  	l = cursor2 - cursor1 - strlen("</Level>\n");
  	for (i = 0; i < l; i++)
    	{
    	c = fgetc(slcFile);
	fprintf(stderr, "%c", c );
    	}
  	fclose(slcFile);
}


void readLevel (int levelNum)
{
   char nameFile[MAX_CARACT] = "levels/12_Blocks.slc";
   char line[MAX_CARACT] = "";
   long cursor1 = 0, cursor2 = 0;
   FILE *slcFile = NULL;
   slcFile = fopen (nameFile, "r");

   /* check if file exist */
   if (slcFile != NULL)
   {
        fprintf (stderr, "%s opened %s\n", nameFile, SDL_GetError ());
   }
  else
    {
          fprintf (stderr, "Can't open %s : %s\n", nameFile, SDL_GetError ());
    }


	int j = 0;
  	fprintf(stderr, "level number :%d\n", levelNum);
	while (fgets(line, MAX_CARACT, slcFile)!= NULL && j != levelNum)
	    {
		  	 /* mark beginning */
	      		if(strstr(line, "<Level") != NULL)
			{
			     	cursor1 = ftell(slcFile);
			}
	      		/* mark end */
		      	if(strstr(line, "</Level>") != NULL)
			{
			     	cursor2 = ftell(slcFile);
			  	j++;
			}

	    }
      /* extract content between two marks */
  	fseek(slcFile, cursor1, SEEK_SET);
	int i = 0, k= 0,c = 0, l = 0;
  	char lineLevel[100] = "";
  	const char start[] = "<L>";
  	char *token;
  	l = cursor2 - cursor1 - strlen("</Level>\n");
  	for (i = 0; i < l; i++)
    	{
	    while (c != '\n')
		{
		c = fgetc(slcFile);
		sprintf(lineLevel, "%c", c);
		token = strtok(lineLevel, start);
		while(token != NULL)
		    {
		      	fprintf(stderr, "%s", lineLevel);
		      	token = strtok(NULL, start);
		    }


		}

    	}

  	fclose(slcFile);
}
#endif
