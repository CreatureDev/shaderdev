#include "SDL_ttf.h"
#include "SDL_events.h"
#include "SDL_render.h"
#include "SDL_surface.h"
#include "shadeutil-2.h"
#include <stdio.h>
#include <stdlib.h>

#define DFLTFONT "/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf"

/* modes
 *	CONTROL 
 *	RUN
 *	SHADER MANAGEMENT
 *	INSTANCE MANAGEMENT
 *	BUFFER MANAGEMENT
 *	TEXTURE MANAGEMENT
 *
*/

/* program state machine */
static SDL_Renderer *rend = 0;
static TTF_Font *fnt = 0;
static int errcode = 0;
/* end program state machine */



int initgui()
{
	errcode = 0;
	extern SDL_Window *wind;
	if(rend)
		return errcode;
	if(!wind || TTF_Init())
		return (errcode = -1);
	rend = SDL_CreateRenderer(wind, -1, SDL_RENDERER_ACCELERATED);
	if(!rend)
		return (errcode = -2);
	
	fnt = TTF_OpenFont(DFLTFONT, 12);
	if(!fnt)
		return (errcode = -3);

	return 0;
}

