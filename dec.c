#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

SDL_Surface *screen, *tmp, *scr;

unsigned char* buffer;

unsigned short inc = 0;
unsigned char format = 0;

void fskip(FILE *fp, int num_bytes)
{
   int i;
   for (i=0; i<num_bytes; i++)
      fgetc(fp);
}


int main(int argc, char* argv[])
{
	FILE* fp;
	SDL_Event event;
	int quit = 0;
	int i;
	SDL_Color *palette;
	unsigned short width;
	unsigned short height;
	unsigned int color_hold;
	unsigned long long file_pos = 0;
	SDL_Rect rct;
	unsigned long long lSize = 0;
	int w, h;
	SDL_Init(SDL_INIT_VIDEO);
	screen = SDL_SetVideoMode(320, 240, 8, SDL_HWSURFACE);
	
	fp = fopen(argv[1], "rb");
	
	// obtain file size:
	fseek (fp , 0 , SEEK_END);
	lSize = ftell(fp) - 5;
	rewind (fp);
	
	fread(&format, 1, sizeof(unsigned char), fp);
	fseek(fp,1,SEEK_SET);
	fread(&width, 1, sizeof(unsigned short), fp);
	fseek(fp,3,SEEK_SET);
	fread(&height, 1, sizeof(unsigned short), fp);
	fseek(fp,5,SEEK_SET);

	screen = SDL_SetVideoMode(320, 240, 8, SDL_HWSURFACE);
	tmp = IMG_Load(argv[2]);
	screen = SDL_SetVideoMode(tmp->w, tmp->h, 8, SDL_HWSURFACE);
	scr = SDL_CreateRGBSurface(SDL_HWSURFACE, tmp->w, tmp->h, 8, 0,0,0,0);
	
	SDL_SetPalette(screen, SDL_LOGPAL|SDL_PHYSPAL, tmp->format->palette->colors, 0, 256);
	SDL_SetPalette(scr, SDL_LOGPAL|SDL_PHYSPAL, tmp->format->palette->colors, 0, 256);
	
	SDL_FillRect(scr, NULL, SDL_MapRGB(tmp->format, 0, 0, 0));
	
	/*for(i=0;i<lSize;i++)
	{
		printf("buffer[%d] %d\n", i, buffer[i]);
	}*/
	//fread(buffer, lSize, 1, fp);
	unsigned short increment = 0;
	unsigned char color = 0;
	unsigned char sized = 0;
	w = 0;
	h = 0;
	file_pos = 5;
	
	if (format == 0)
	{
		sized = sizeof(unsigned char);
	}
	else
	{
		sized = sizeof(unsigned short);
	}
	
	for(i=0;i<lSize;i++)
	{
		fread(&increment, 1, sized, fp);
		file_pos += sized;
		fseek(fp,file_pos,SEEK_SET);
		
		fread(&color, 1, sizeof(unsigned char), fp);
		file_pos += sizeof(unsigned char);
		fseek(fp,file_pos,SEEK_SET);
		
		rct.x = w;
		rct.y = h;
		rct.w = increment;
		rct.h = 1;
		SDL_FillRect(scr, &rct, color);
		
		printf("Inc %d\n", increment);
		printf("color %d\n", color);
		
		w += increment;
		if (w >= tmp->w)
		{
			h++;
			w = 0;
			printf("\n\nLine %d\n\n", h);
		}
		/*if (color_hold != buffer[i])
		{
			//printf("inc %d\n", inc);
			//printf("w %d\n", w);
			//printf("h %d\n", h);
			//printf("color_hold %d\n", color_hold);
			
			rct.x = w;
			rct.y = h;
			rct.w = inc;
			rct.h = 1;
			SDL_FillRect(scr, &rct, color_hold);
			inc = 1;
		}
		else
		{
			inc++;
		}
		color_hold = buffer[i];
		w++;
		if (w > tmp->w)
		{
			h++;
			w = 0;
		}*/
	}
	
	
	fclose(fp);
	
	while(!quit)
	{
		
		while (SDL_PollEvent(&event)) 
		{
			switch(event.type) 
			{
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym) 
					{
						case SDLK_ESCAPE:
							quit = 1;
						break;
						default:
						break;
					}
				break;
				case SDL_QUIT:
					quit = 1;
				break;
			}
		}
		
		SDL_BlitSurface(scr, NULL, screen, NULL);
		SDL_Flip(screen);
	}
	
	
	SDL_FreeSurface(screen);
	SDL_FreeSurface(tmp);
	free(buffer);
	return 0;
}
