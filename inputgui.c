#include "SDL_ttf.h"
#include "SDL_events.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "SDL_surface.h"
#include "shadeutil-2.h"
#include <stdio.h>
#include <stdlib.h>

#define DFLTFNT "/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf"

#define GLSTSHD 0
#define COMPSHD 1
#define VERTSHD 2
#define TECTSHD 3
#define TEVLSHD 4
#define GEOMSHD 5
#define FRAGSHD 6

SDL_Renderer *rend = 0;
TTF_Font *fnt = 0;

struct shaderdesc *shds[7] = {0};
const char *dfltnm[7] = {"GL state", "Compute", "Vertex", 
	"TessCtrl", "TessEval", "Geometry", "Fragment"};

SDL_Rect valbx;
SDL_Rect shabx;
SDL_Rect renbx;

static struct
{
	unsigned int st;
	unsigned int shd;
	
} guist = {0, 0};

static void rendshabx()
{
	int i;
	SDL_Rect box = shabx;
	SDL_Rect dst;
	SDL_Surface *sfc;
	SDL_Texture *txt;
	SDL_Color clr = {255, 255, 255, 255};
	box.w /= 7;
	dst.w = box.w / 2;
	dst.h = box.h / 6;
	dst.y = box.y + (box.h - dst.h) / 2;
	dst.x = box.x + (box.w - dst.w) / 2;
	SDL_SetRenderDrawColor(rend, 255, 0, 109, 255);
	for(i = 0; i < 7; i++)
	{
		if(shds[i])
			sfc = TTF_RenderText_Solid(fnt, shds[i]->name, clr);
		else
			sfc = TTF_RenderText_Solid(fnt, dfltnm[i], clr);
		txt = SDL_CreateTextureFromSurface(rend, sfc);
		SDL_FreeSurface(sfc);
		if(guist.shd == i)
		{
			if(guist.st == 0)
				SDL_SetRenderDrawColor(rend, 0, 21, 63, 255);
			else
				SDL_SetRenderDrawColor(rend, 0, 12, 36, 255);
			SDL_RenderFillRect(rend, &box);
			SDL_SetRenderDrawColor(rend, 255, 0, 109, 255);
		}
		SDL_RenderCopy(rend, txt, 0, &dst);
		SDL_RenderDrawRect(rend, &box);
		
		box.x += box.w;
		dst.x += box.w;
		SDL_DestroyTexture(txt);
	}
}

static void rendvalbx()
{
	SDL_SetRenderDrawColor(rend, 50, 205, 50, 255);
	SDL_RenderDrawRect(rend, &valbx);
}

int initgui()
{
	extern SDL_Window *wind;
	float f = 1.f / 5.f;
	int w, h;
	

	if(rend)
		return 0;
	if(!wind || TTF_Init())
	{
		puts("failed to initialized sdl ttf");
		return -1;
	}
	
	rend = SDL_CreateRenderer(wind, -1, SDL_RENDERER_ACCELERATED);
	if(!rend)
	{
		puts("failed to create gui renderer");
		return -1;
	}
	
	SDL_GetWindowSize(wind, &w, &h);
	valbx.x = valbx.y = 0;
	valbx.w = w * f;
	valbx.h = h;
	
	shabx.x = renbx.x = valbx.w;
	renbx.y = 0;
	shabx.h = h * f;
	shabx.y = renbx.h = h - shabx.h;
	shabx.w = renbx.w = w - valbx.w;

	fnt = TTF_OpenFont(DFLTFNT, 12);
	if(!fnt)
	{
		puts("failed to open da font");
		return -1;
	}

	rendshabx();
	rendvalbx();
	return 0;
}



