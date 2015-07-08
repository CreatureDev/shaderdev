#include "SDL.h"
#include "shadeutil.h"
#include <stdio.h>

static SDL_Window *wind = 0;
static SDL_GLContext glct = 0;

void initwindow()
{
	int i, j;
	i = 4;
	j = 5;
	while(i)
	{
		if(wind)
		{
			SDL_DestroyWindow(wind);
			wind = 0;
		}
		SDL_GL_ResetAttributes();
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, i);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, j);
		wind = SDL_CreateWindow("parsetest",
				SDL_WINDOWPOS_UNDEFINED,
				SDL_WINDOWPOS_UNDEFINED,
				0, 0,
				SDL_WINDOW_FULLSCREEN_DESKTOP |
				SDL_WINDOW_OPENGL);

		glct = SDL_GL_CreateContext(wind);

		if(glct)
			break;
		if(!j)
		{
			i--;
			j = 5;
		}
		else
		{
			j--;
		}
	}
}

int main(int argc, char **argv)
{

	if(SH_init())
	{
		printf("failed to initialize shader util\n");
		return -1;
	}
	if(SDL_Init(SDL_INIT_VIDEO))
	{
		printf("failed to initialize SDL\n");
		return -1;
	}

	initwindow();
		

	for(argc = 0; argc < 100; argc++)
		SDL_GL_SwapWindow(wind);
		
	return 0;
}

