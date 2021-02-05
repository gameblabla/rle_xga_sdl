#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

#define FORMAT_SHORT 1

SDL_Surface *screen, *tmp;

unsigned short inc = 0;
unsigned char format = 0;


Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
	return *p;
}


int main(int argc, char* argv[])
{
	FILE* fp;
	SDL_Event event;
	int quit = 0;
	int i,a;
	unsigned char color_hold = 0;
	unsigned long long file_pos = 0;
	SDL_Color *palette;
	
	SDL_Init(SDL_INIT_VIDEO);
	tmp = IMG_Load(argv[1]);
	SDL_SetPalette(screen, SDL_LOGPAL|SDL_PHYSPAL, tmp->format->palette->colors, 0, 256);

	fp = fopen(argv[2], "wb");
	
	format = 2;
	/* Format also supports a smaller size mode but this may not be doable depending
	 * on the image format itself. */
	if (argc > 3)
	{
		if (argv[3][0] == 's')
		{
			format = 1;
		}
	}
	
	fwrite(&format, 1, sizeof(unsigned char), fp);
	fwrite(&tmp->w, 1, sizeof(unsigned short), fp);
	fwrite(&tmp->h, 1, sizeof(unsigned short), fp);
	
	file_pos = 5;
	for(i=0;i<=tmp->h;i++)
	{
		a = 0;
		color_hold = getpixel(tmp, a, i);
		inc = 0;
		for(a=0;a<=tmp->w;a++)
		{
			if (color_hold != getpixel(tmp, a, i))
			{
				fwrite(&inc, 1, format, fp);
				fwrite(&color_hold, 1, sizeof(color_hold), fp);
				inc = 1;
			}
			else
			{
				inc++;
			}
			color_hold = getpixel(tmp, a, i);
		}
	}
	fclose(fp);
	SDL_FreeSurface(screen);
	SDL_FreeSurface(tmp);
	return 0;
}
