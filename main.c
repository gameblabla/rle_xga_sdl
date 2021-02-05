#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

SDL_Surface *screen, *tmp;

unsigned char format = 0;

Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
	return *p;
}

int RLE_encoder(const char* filetoopen, SDL_Surface* tmp)
{
	FILE *fp;
	int i,a;
	unsigned char color_hold = 0;
	unsigned short inc = 0;
	unsigned char err = 0;
	fp = fopen(filetoopen, "wb");
	
	fwrite(&format, 1, sizeof(unsigned char), fp);
	fwrite(&tmp->w, 1, sizeof(unsigned short), fp);
	fwrite(&tmp->h, 1, sizeof(unsigned short), fp);
	
	/* RIP : Gameblabla. Thanks irixxxx for fixing my encoder */
	for(i=0;i<tmp->h;i++)
	{
		color_hold = getpixel(tmp, 0, i);
		inc = 1;
		// a is set to 1 since we are already reading the pixel before entering the loop
		// Also we use < instead of <= to avoid overdrawing especially after we write out the rest.
		for(a=1;a<tmp->w;a++)
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
				/* Check for overdraw in 8 bits increment mode */
				if (format == 1)
				{
					if (inc > 255)
					{
						err = 1;
						i = tmp->h;
					}
				}
			}
			color_hold = getpixel(tmp, a, i);
		}
		// Write out the remaining pixels that may have not written
		fwrite(&inc, 1, format, fp);
		fwrite(&color_hold, 1, sizeof(color_hold), fp);
	}
	fclose(fp);
	
	return err;
}

int main(int argc, char* argv[])
{
	FILE* fp;
	SDL_Event event;
	int quit = 0;
	SDL_Color *palette;
	int fail = 0;
	
	SDL_Init(SDL_INIT_VIDEO);
	tmp = IMG_Load(argv[1]);
	SDL_SetPalette(screen, SDL_LOGPAL|SDL_PHYSPAL, tmp->format->palette->colors, 0, 256);

	format = 1;
	fail = RLE_encoder(argv[2], tmp);
	
	if (fail > 0)
	{
		printf("It failed with char, upgrading to using shorts instead.\nFile size will be increased as a result\n");
		format = 2;
		fail = RLE_encoder(argv[2], tmp);
	}
	
	SDL_FreeSurface(screen);
	SDL_FreeSurface(tmp);
	return 0;
}
