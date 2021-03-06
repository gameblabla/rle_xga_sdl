#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

SDL_Surface *screen, *tmp, *scr;
unsigned short inc = 0;
unsigned char format = 0;


int main(int argc, char* argv[])
{
	FILE* fp;
	SDL_Event event;
	int quit = 0, i;
	SDL_Color *palette;
	unsigned short width;
	unsigned short height;
	unsigned short increment = 0;
	unsigned char color = 0;
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
	fread(&width, 1, sizeof(unsigned short), fp);
	fread(&height, 1, sizeof(unsigned short), fp);

	screen = SDL_SetVideoMode(320, 240, 8, SDL_HWSURFACE);
	tmp = IMG_Load(argv[2]);
	screen = SDL_SetVideoMode(1280, 720, 8, SDL_HWSURFACE);
	scr = SDL_CreateRGBSurface(SDL_HWSURFACE, 1280, 720, 8, 0,0,0,0);
	
	SDL_SetPalette(screen, SDL_LOGPAL|SDL_PHYSPAL, tmp->format->palette->colors, 0, 256);
	SDL_SetPalette(scr, SDL_LOGPAL|SDL_PHYSPAL, tmp->format->palette->colors, 0, 256);
	
	SDL_FillRect(scr, NULL, SDL_MapRGB(tmp->format, 0, 0, 0));

	w = 0;
	h = 0;
	for(i=0;i<lSize;i++)
	{
		fread(&increment, 1, format, fp);
		fread(&color, 1, sizeof(unsigned char), fp);
		rct.x = w;
		rct.y = h;
		rct.w = increment;
		rct.h = 1;
		SDL_FillRect(scr, &rct, color);
		w += increment;
		if (w >= tmp->w)
		{
			h++;
			w = 0;
		}
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
	return 0;
}
