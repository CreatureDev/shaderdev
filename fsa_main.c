#include "SDL.h"
#include "SDL_ttf.h"
#include <stdlib.h>
#include <stdio.h>

#define err_state(x) (((x) >> 31) & 1)
#define err_ptr(x) ((int) ((unsigned long) x / ((unsigned long) x - 1)) - 1)

static int m_st = 0;
static SDL_Window *wind = 0;
static SDL_Renderer *rend = 0;
static SDL_GLContext glct = 0;

static int build_context()
{
	SDL_GL_ResetAttributes();
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
	wind = SDL_CreateWindow("shade_dfa",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			0, 0,
			SDL_WINDOW_FULLSCREEN_DESKTOP |
			SDL_WINDOW_OPENGL);

	glct = SDL_GL_CreateContext(wind);

	rend = SDL_CreateRenderer(wind, 0, 0);
	
	return (err_ptr(wind) | err_ptr(rend) | err_ptr(glct));
}

int main(int argc, char **argv)
{
	void *jtb[2] = {&&GOOD, &&ERRO};
	m_st = err_state(SDL_Init(SDL_INIT_EVERYTHING));
	goto *jtb[m_st];
GOOD:
	jtb[0] = &&INIT;
	m_st = err_state(build_context());
	goto *jtb[m_st];
INIT:
	puts("good");
	return 0;
ERRO:
	puts(SDL_GetError());
	return -1;
}

