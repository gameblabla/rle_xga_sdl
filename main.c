#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

SDL_Surface *screen, *tmp;

unsigned char* buffer;

#ifdef FORMAT_SHORT
unsigned short inc = 0;
unsigned char format = 1;
#else
unsigned char inc = 0;
unsigned char format = 0;
#endif

void fskip(FILE *fp, int num_bytes)
{
   int i;
   for (i=0; i<num_bytes; i++)
      fgetc(fp);
}

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
	int w;
	
	SDL_Init(SDL_INIT_VIDEO);
	screen = SDL_SetVideoMode(320, 240, 8, SDL_HWSURFACE);
	tmp = IMG_Load(argv[1]);
	screen = SDL_SetVideoMode(tmp->w, tmp->h, 8, SDL_HWSURFACE);
	SDL_SetPalette(screen, SDL_LOGPAL|SDL_PHYSPAL, tmp->format->palette->colors, 0, 256);
	
	buffer = malloc(tmp->w*tmp->h);
	memcpy(buffer, tmp->pixels, tmp->w*tmp->h);
	
	/*while(!quit)
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
		
		SDL_BlitSurface(tmp, NULL, screen, NULL);
		SDL_Flip(screen);
	}*/

	inc = 1;
	fp = fopen(argv[2], "wb");
	color_hold = 0;
	
	fwrite(&format, 1, sizeof(unsigned char), fp);
	fseek(fp,1,SEEK_SET);
	fwrite(&tmp->w, 1, sizeof(unsigned short), fp);
	fseek(fp,3,SEEK_SET);
	fwrite(&tmp->h, 1, sizeof(unsigned short), fp);
	fseek(fp,5,SEEK_SET);
	
	color_hold = getpixel(tmp, 0, 0);
	w = 0;
	a = 0;
	i = 0;
	inc = 0;
	file_pos = 5;
	for(i=0;i<=tmp->h;i++)
	{
		a = 0;
		color_hold = getpixel(tmp, a, i);
		printf("\n\nNext line %d \n\n", i);
		for(a=0;a<=tmp->w;a++)
		{
			if (color_hold != getpixel(tmp, a, i))
			{
				fwrite(&inc, 1, sizeof(inc), fp);
				file_pos += sizeof(inc);
				fseek(fp,file_pos,SEEK_SET);
				fwrite(&color_hold, 1, sizeof(color_hold), fp);
				file_pos += 1;
				fseek(fp,file_pos,SEEK_SET);
				
				printf("inc %d, color_hold %d, x %d\n", inc, color_hold, a);
				inc = 1;
			}
			else
			{
				inc++;
			}
			color_hold = getpixel(tmp, a, i);
		}
	}
	
	/*for(i=0;i<tmp->w*tmp->h;i++)
	{

	}*/
	
	//fprintf(fp, "%d", color_hold);
	fclose(fp);
	
	
	SDL_FreeSurface(screen);
	SDL_FreeSurface(tmp);
	free(buffer);
	return 0;
}
