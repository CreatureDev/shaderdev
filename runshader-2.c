#include "inputgui.h"
#include "shadeutil-2.h"
#include "SDL.h"
#include "gl.h"
#include "glext.h"
#include <stdio.h>
#include <stdlib.h>

SDL_Window *wind = 0;
SDL_GLContext glct = 0;

void initwindow(const char *shnm)
{
	int i, j;
	char buff[1024];

	sprintf(buff, "shader test: %s", shnm);

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
		wind = SDL_CreateWindow(buff,
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
	if(!i)
	{
		printf("Failed to create an OpenGL context\n");
		exit(-1);
	}
}

int main(int argc, char **argv)
{
	if(SDL_Init(SDL_INIT_EVERYTHING))
	{
		puts("failed to initialize SDL");
		return -1;
	}
	initwindow("v2");
	
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	if(initgui())
	{
		puts("failed to initialze GUI");
		return -1;
	}
	

	SDL_GL_SwapWindow(wind);

	SDL_Delay(1000);

	SDL_Quit();

	return 0;
}

